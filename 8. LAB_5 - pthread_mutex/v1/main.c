/* Тема: Потоковые мьютексы[pthread_mutex];
 * Дата: 28.11.2021 г.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>

#define FILE_NAME "main.c"
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12

struct DATA
{
	int SIZE;
	int* mas;
	int count;
	
	pthread_t* pth_read_tid_s;
	pthread_t pth_write_tid;
	pthread_mutex_t mutex;
};

//pthread_mutex_t mutex;

void my_handler(int signum)
{
    if (signum == SIGUSR1)
    {
        pause();// Если система через pthrread_kill получает в стек сигналов 
		// определяемый пользователем сигнал SIGUSR1, то вызывается 
		// обработчик сигналов my_handler() и с помощью pouse() поток 
		// ставится на паузу до тех пор пока не будет вызван обработчик 
		// сигналов или не будет вызвана команда завершения процесса.

    }else if(signum == SIGUSR2){
	// Здесь ничего не реализуется. Это условие дополнительно. 
	// Основная суть улавить сигнал, чтобы поток который стоит 
	// на паузе мог возобновить работу.
    }
}

void* pTh_fun_write(void* data)
{
	for(int i = 0; i < ((struct DATA*)data)->SIZE; i++)
	{
		pthread_mutex_lock(&((struct DATA*)data)-> mutex);
	
		((struct DATA*)data)->mas[i] = (((struct DATA*)data)->count += 1);
		
		pthread_kill( ((struct DATA*)data)->pth_read_tid_s[i], SIGUSR2);

		pthread_mutex_unlock(&((struct DATA*)data)->mutex);

		pthread_kill(((struct DATA*)data)->pth_write_tid, SIGUSR1);
		
		sleep(1);
		
		//fflush(NULL);// В некоторых случаях 
		//требуется поместить сигнал в стек сигналов 
		//и сразу выполнить этот сигнал. 
		//В нашем случае этого не требуется т. к. 
		//нужно, чтобы текущий поток был удален.
	}
	
	pthread_exit(NULL);
}

void* pTh_fun_read(void* data)
{
	pthread_mutex_lock(&((struct DATA*)data)-> mutex);

		printf("[TID: \033[1;31m%lu\033[0m]\n",  pthread_self());

	for(int i = 0; i < ((struct DATA*)data)->SIZE; i++){
		printf("MAS[\033[1;32m%d\033[0m] = \033[1;33m%d\033[0m ; \n", i, ((struct DATA*)data)->mas[i]);
	}
	//fflush(NULL);

	pthread_mutex_unlock(&((struct DATA*)data)-> mutex);

	pthread_kill(((struct DATA*)data)->pth_write_tid, SIGUSR2);	// Возобновляем работу потока, который ранее был поставлен на паузу.
	
	pthread_exit(NULL);
}

int main(void)
{
	signal(SIGUSR1, my_handler); // Регистрация сигнала паузы. 
	signal(SIGUSR2, my_handler); // Регистрация сигнала для возобновления.

	struct DATA data_1;

	data_1.SIZE = 10;
	data_1.count = 0;

	data_1.mas = (int*) malloc( data_1.SIZE * sizeof(int));

	data_1.pth_read_tid_s  = (pthread_t*) malloc( data_1.SIZE * sizeof(pthread_t));

	pthread_create( &data_1.pth_write_tid, NULL,  pTh_fun_write, (void*)&data_1 );
	
	pthread_mutex_init(&data_1.mutex, NULL);

	pthread_mutex_lock(&data_1.mutex);
	
	//pthread_detach(data_1.pth_write_tid);

	for(int i = 0; i < data_1.SIZE; i++){

		int crt_status = pthread_create( &data_1.pth_read_tid_s[i], NULL,  pTh_fun_read, (void*)&data_1 );
		if (crt_status != 0) {
        		printf("main error: can't create thread, status = %d\n", crt_status);
        		exit(ERROR_CREATE_THREAD);
    		}else{
			//printf("TID: %lu \n", data_1.pth_read_tid_s[i] );
		}

		pthread_kill( data_1.pth_read_tid_s[i], SIGUSR1); // Ставим поток на поузу.
	}

	pthread_mutex_unlock(&data_1.mutex);
	
	pthread_join(data_1.pth_write_tid, NULL);
	
	pthread_cond_destroy(&data_1.cond);
    pthread_mutex_destroy(&data_1.mutex);

	return 0;
}
 
