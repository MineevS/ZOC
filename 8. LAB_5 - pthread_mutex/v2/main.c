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
	
} DATA;

void c_u_h(void *data)
{
	pthread_mutex_unlock(&((struct DATA*)data)-> mutex);
	
	pause();
	
	pthread_exit(NULL);
}

void* pTh_fun_read(void* data)
{
	pthread_mutex_lock(&((struct DATA*)data)-> mutex);
	
	//pthread_cleanup_push(cancel_unlock_handler, (void*)((struct DATA*)data)-> mutex);
	pthread_cleanup_push(c_u_h, data);

	printf("[TID: \033[1;31m%lx\033[0m]\n",  pthread_self());

	for(int i = 0; i < ((struct DATA*)data)->SIZE; i++){
		printf("MAS[\033[1;32m%d\033[0m] = \033[1;33m%d\033[0m ; \n", i, ((struct DATA*)data)->mas[i]);
	}
	
	//pthread_mutex_unlock(&((struct DATA*)data)-> mutex);
	
	//pthread_kill(((struct DATA*)data)->pth_write_tid, SIGUSR2);	// Возобновляем работу потока, который ранее был поставлен на паузу.
	//pthread_exit(NULL);
	
	pthread_cleanup_pop(1);// очищает стек вызов текущего потока без вызова функции завершения c_u_h.
	
	return NULL;
}

void* pTh_fun_write(void* data)
{
	for(int i = 0; i < ((struct DATA*)data)->SIZE; i++)
	{
		pthread_mutex_lock(&((struct DATA*)data)-> mutex);
		
		((struct DATA*)data)->mas[i] = (((struct DATA*)data)->count += 1);
		
		int crt_status = pthread_create( &((struct DATA*)data)->pth_read_tid_s[i], NULL,  pTh_fun_read, data );
		if (crt_status != 0) {
        		printf("main error: can't create thread, status = %d\n", crt_status);
        		exit(ERROR_CREATE_THREAD);
    		}else{
			//printf("TID: %lu \n", data_1.pth_read_tid_s[i] );
		}

		pthread_mutex_unlock(&((struct DATA*)data)->mutex);
		
		sleep(1);// Задержка на то, чтобы поток на чтение успел вывести информацию.
	}
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

	pthread_create( &data_1.pth_write_tid, NULL,  pTh_fun_write, (void*)&data_1 );
	
	pthread_join(data_1.pth_write_tid, NULL);

    pthread_mutex_destroy(&data_1.mutex);
	
	return 0;
}

// Вывод: Данный способ без использования сигналов более правильный при использовании мьютексов. 
// Недостатки: 1) Но при этом способе на экране имеются мерцания вывода на экран. 
//			   2) И потоки по времени имеют задержки работы в 1 секунду.