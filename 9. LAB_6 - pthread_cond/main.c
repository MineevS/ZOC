/* Тема: Потоковые мьютексы[pthread_mutex];
 * Дата: 28.11.2021 г.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <pthread.h>
#include <signal.h>

#define FILE_NAME "main.c"
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12

typedef struct DATA
{
	int SIZE;
	int* mas;
	int count;
	
	pthread_t* pth_read_tid_s;
	pthread_t pth_write_tid;
	
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	
} DATA;

void c_u_h(void *data)
{
	pthread_exit(NULL);
}

void* pTh_fun_read(void* data)
{
	pthread_cond_signal(&((struct DATA*)data)->cond);// Получение сигнала от pthread_cond_wait() 
	// и разблокировка мьютекса после того как поток очистит стек вызовов. 
	// с помечанием, чтобы потом опять pthread_cond_wait() его заблокировал.
	
	pthread_cleanup_push(c_u_h, NULL);

	printf("[TID: \033[1;31m%lx\033[0m]\n",  pthread_self());

	for(int i = 0; i < ((struct DATA*)data)->SIZE; i++){
		printf("MAS[\033[1;32m%d\033[0m] = \033[1;33m%d\033[0m ; \n", i, ((struct DATA*)data)->mas[i]);
	}
	
	pthread_cleanup_pop(1);// очищает стек вызов текущего потока без вызова функции завершения c_u_h.
	
	return NULL;
}

void* pTh_fun_write(void* data)
{
	pthread_mutex_lock(&((struct DATA*)data)-> mutex);
	
	int step = 0;
	
	for(int i = 0; i < ((struct DATA*)data)->SIZE; i++)
	{
		((struct DATA*)data)->mas[i] = (((struct DATA*)data)->count += 1);
		
		if((((struct DATA*)data)->count) >= step){
			pthread_cond_wait(&((struct DATA*)data)->cond, &((struct DATA*)data)->mutex);//Разблокируем мьютекс.
		}
		
		step = ((struct DATA*)data)->count;
	}
	
	pthread_mutex_unlock(&((struct DATA*)data)->mutex);

	return NULL;
}

struct DATA data_1;

int main(void)
{
	data_1.SIZE = 10;
	data_1.count = 0;
	data_1.mas = (int*) malloc( data_1.SIZE * sizeof(int));
	data_1.pth_read_tid_s  = (pthread_t*) malloc( data_1.SIZE * sizeof(pthread_t));
	
	pthread_mutex_init(&data_1.mutex, NULL);
	pthread_cond_init(&data_1.cond, NULL);

	pthread_create(&data_1.pth_write_tid, NULL,  pTh_fun_write, (void*)&data_1);
	pthread_detach(data_1.pth_write_tid);

	// Все потоки должны быть созданны в основном потоке т. к. ОС Linux выдяляет АП 
	// для процесса и процесс распределяет АП между всеми потоками, которые созданны только главным(основным) потоком.
	for(int i = 0; i < data_1.SIZE; i++){

		pthread_mutex_lock(&data_1.mutex);
		
		int crt_status = pthread_create( &data_1.pth_read_tid_s[i], NULL,  pTh_fun_read, (void*)&data_1 );
		if (crt_status != 0) {
        		printf("main error: can't create thread, status = %d\n", crt_status);
        		exit(ERROR_CREATE_THREAD);
    		}else{
			//printf("TID: %lu \n", data_1.pth_read_tid_s[i] );
		}
		
		pthread_mutex_unlock(&data_1.mutex);
		// Сразу после разблокировки мьютекса отрабатывает созданный поток.
		// Чтобы пока созданный поток отрабатывает не запустился текущий 
		// приходится текущий либо ставить на паузу либо делать ожидание, 
		// пока не завершится созданный поток. Во втором случае появляются не очень хорошие ситуации.
		sleep(1);	// Вынужденное использование задержки, а не необходимое.
	}
	
	pthread_cond_destroy(&data_1.cond);
    pthread_mutex_destroy(&data_1.mutex);
	
	free(data_1.mas);
	
	return 0;
}

// Вывод: Данный способ содержит меньше sleep(), что повышает работу потоков. 
// Также имеется условие при котором может разблокироваться поток в записывающем 
// потоке для того, чтобы мог прочитать информацию читающий поток. 
// Недостатки: 1) Наличие одного вынужденного sleep().