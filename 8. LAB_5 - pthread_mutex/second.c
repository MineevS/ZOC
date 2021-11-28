/*Тема: Потоковые Мьютексы[pthread_mutex];
* Date: 28.11.2021;
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/timeb.h>

#define FILE_NAME "buff_mem_1"
#define SHM_ID 1

typedef struct data_struct
{
	time_t ttime;
	pid_t pid;

}data_struct;

struct sembuf sem_lock = { 0 /*Номер семафора*/,
                          -1 /*Операция над семафором*/,
                           1 /*Флаг операции*/ },
              sem_open = { 0 /*Номер семафора*/,
                           1 /*Операция над семафором*/,
                           0 /*Флаг операции*/ };

char* timeTostring(time_t ttime)
{
	struct tm *m_time = localtime(&ttime);
	static char str_t[128]="";
	memset(str_t, 0, sizeof(char)*128);
	strftime (str_t, 128, "%x %A %X %Z", m_time); 

	return str_t;
}

int main(){
	sleep(1);
	
	// Получение токена:
	key_t ipc_key = ftok(FILE_NAME, SHM_ID);

	if(ipc_key == -1){
		perror("ftok error!");
		exit(-1);
	}

	//Создание SHM:
	int shmem_id = shmget(ipc_key, sizeof(data_struct), 0666);

	if(shmem_id == -1){
		perror("shmget error!\n");
		exit(-2);
	}else{
		printf("PROGRAMM: \033[1;36m2\033[0m\n");
		
		int sem_id = semget(ipc_key, 1*sizeof(data_struct), IPC_CREAT | 0666);

		void* ptr_to_shm = shmat(shmem_id, NULL, 0);
		
		if(ptr_to_shm == (void*)(-1)){
			printf("SHMEM attach failed with error: %s\n", strerror(errno));
			exit(-3);
		}else if(sem_id == -1){
			 printf("semget error: %s\n", strerror(errno));
			 exit(-6);
		}else{
			sleep(2);

			semop(sem_id, &sem_lock, 1);
			printf("Transmitted data in shm:\n" 
				"[ttime: \033[31m%s\033[0m]\n "
				"[pid: \033[31m%d\033[0m]\n", 
				timeTostring(((data_struct*)ptr_to_shm)->ttime),
				((data_struct*)ptr_to_shm)->pid);

			semop(sem_id, &sem_open, 1);

			printf("The time of the end of the reception data: \033[1;33m%s\033[0m\n",
				 timeTostring(time(0)));
			shmdt(ptr_to_shm);
		}
	}
	return 0;
}
