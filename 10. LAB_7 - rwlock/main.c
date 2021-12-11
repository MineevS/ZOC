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

	pthread_rwlock_t rwlock;

} DATA;

void c_u_h(void *data)
{
	//printf("[TID_ 2: \033[1;31m%lx\033[0m]\n",  pthread_self());
	
	//printf("[RWLOCK_2_R_1: \033[1;31m%d\033[0m]\n",  ((struct DATA*)data)->rwlock.__data.__nr_readers);
	//printf("[RWLOCK_2_W_1: \033[1;31m%d\033[0m]\n",  ((struct DATA*)data)->rwlock.__data.__writer);
	
	pthread_rwlock_unlock(&((struct DATA*)data)->rwlock);
	
	//printf("[RWLOCK_2_R_2: \033[1;31m%d\033[0m]\n",  ((struct DATA*)data)->rwlock.__data.__nr_readers);
	//printf("[RWLOCK_2_W_2: \033[1;31m%d\033[0m]\n",  ((struct DATA*)data)->rwlock.__data.__writer);
	
	pause();// Пауза для завершающего потока, чтобы pthread_create() для 
	//нового потока сгенерировал новый tid а не брал tid уже существующего потока и оптимизировал работу с потоками.
	
	pthread_exit(NULL);
}

void* pTh_fun_read(void* data)
{
	pthread_rwlock_rdlock(&((struct DATA*)data)->rwlock);
	
	//printf("[TID_i: \033[1;31m%lx\033[0m]\n",  pthread_self());
		
	//printf("[RWLOCK_i: \033[1;31m%d\033[0m]\n",  ((struct DATA*)data)->rwlock.__data.__nr_readers);
	
	if((((struct DATA*)data)->rwlock.__data.__writer == 0) && (((struct DATA*)data)->rwlock.__data.__nr_readers <= 1)){
		
		pthread_cleanup_push(c_u_h, data);

		//printf("[TID_1: \033[1;31m%lx\033[0m]\n",  pthread_self());
		
		//printf("[RWLOCK_1: \033[1;31m%d\033[0m]\n",  ((struct DATA*)data)->rwlock.__data.__nr_readers);
		
		printf("[TID: \033[1;31m%lx\033[0m]\n",  pthread_self());

		for(int i = 0; i < ((struct DATA*)data)->SIZE; i++){
			printf("MAS[\033[1;32m%d\033[0m] = \033[1;33m%d\033[0m ; \n", i, ((struct DATA*)data)->mas[i]);
		}
		
		pthread_cleanup_pop(1);// очищает стек вызов текущего потока без вызова функции завершения c_u_h.
	}else{
		pthread_rwlock_unlock(&((struct DATA*)data)->rwlock);
	}
	
	
	
	return NULL;
}

void* pTh_fun_write(void* data)
{
	for(int i = 0; i < ((struct DATA*)data)->SIZE; i++)
	{
		pthread_rwlock_wrlock(&((struct DATA*)data)->rwlock);
		
		//printf("[TID_0: \033[1;31m%lx\033[0m]\n",  pthread_self());
		
		//printf("[RWLOCK_0_W: \033[1;31m%d\033[0m]\n",  ((struct DATA*)data)->rwlock.__data.__writer);
		//printf("[RWLOCK_0_R: \033[1;31m%d\033[0m]\n",  ((struct DATA*)data)->rwlock.__data.__nr_readers);

			((struct DATA*)data)->mas[i] = (((struct DATA*)data)->count += 1);
			
			int crt_status = pthread_create( &((struct DATA*)data)->pth_read_tid_s[i], NULL,  pTh_fun_read, data );
			if (crt_status != 0) {
					printf("main error: can't create thread, status = %d\n", crt_status);
					exit(ERROR_CREATE_THREAD);
				}else{
				//printf("TID: %lu \n", data_1.pth_read_tid_s[i] );
			}
			
			pthread_detach(((struct DATA*)data)->pth_read_tid_s[i]);
			// Отсоединенные потоки поочередно сразу после соданию перед текущем потоком могут иметь одинаковые tid.
		 
		pthread_rwlock_unlock(&((struct DATA*)data)->rwlock);
		
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
	
	pthread_rwlock_init(&data_1.rwlock,NULL);

	pthread_create( &data_1.pth_write_tid, NULL,  pTh_fun_write, (void*)&data_1 );
	
	pthread_join(data_1.pth_write_tid, NULL);
	
	return 0;
}

// Вывод: Данный способ позволяет отделить потоки по набора на запись и чтение. 
// Недостатки: 1) Требуется dsye;ltyyst sleep() для синхронизации потоков. 
//			   2) И потоки по времени имеют задержки работы в 1 секунду.
// Пока читаем не сможем записать.
