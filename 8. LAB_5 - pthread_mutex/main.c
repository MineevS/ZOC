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

struct DATA
{
        int SIZE;
        int* mas;
        int count;
	pthread_t* pth_read_tid_s;
	pthread_t pth_write_tid;
};

pthread_mutex_t lock;

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
		pthread_mutex_lock(&lock);

		((struct DATA*)data)->mas[i] = (((struct DATA*)data)->count += 1);

		pthread_mutex_unlock(&lock);
		
		pthread_kill(pthread_self(), SIGUSR1);// Посылаем сигнал "Болванку", 
						      // для того, чтобы записывающий 
						      // поток мог возобновить работу.
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
	pthread_mutex_lock(&lock);

		printf("[TID: \033[1;31m%lu\033[0m]\n", pthread_self() );

	for(int i = 0; i < ((struct DATA*)data)->SIZE; i++){
		printf("MAS[\033[1;32m%d\033[0m] = \033[1;33m%d\033[0m ; \n", i, ((struct DATA*)data)->mas[i]);
	}

	pthread_mutex_unlock(&lock);
	
	pthread_kill(((struct DATA*)data)->pth_write_tid, SIGUSR2);
	
	//fflush(NULL);

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
	//pthread_join( pth_write_tid, NULL );// Ожидания Завершения работы этого потока не требуется. 
					      // т. к. если мы будем ожидать, то основной поток будет 
					      // находится в ожидании и не сможет создать потоки на чтение.

	// переводим в отсоединенный режим
	pthread_detach(data_1.pth_write_tid);// Отделяем записывающий поток от основного.

	for(int i = 0; i < data_1.SIZE; i++){

		pthread_create( &data_1.pth_read_tid_s[i], NULL,  pTh_fun_read, (void*)&data_1 );
		pthread_join( data_1.pth_read_tid_s[i], NULL ); // Ожидание пока поток завершит работу.
		
		sleep(1); // Небольшая задержка текущего потока, 
			  // чтобы успеть проконтрольровать весь вывод информации в консоль.
	}

	 fflush(NULL);// Очистка стека сигналов. Если не очистить стек сигналов, 
		      // то при повторном запуске программы, программа получит 
		      // последний сигнал из стека и при обработке сначало может 
		      // запуститься поток на чтения, а уже потом на запись, 
		      // чего бы нам хотелось избежать.

	return 0;
}
 
