/*Тема: Семафоры[Semaphor(s)];
* Date: 28.11.2021;
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
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

} data_struct;

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

int main(int argc, char** argv)
{
	int LEN = 256;
	char line[256]= "\0";
	FILE *cmd = popen("pidof Prog_1", "r");

	if(cmd == NULL){

		printf("Error cmd!\n");
		exit(-1);
	}

	fgets(line, LEN, cmd);
	pid_t pid_id = strtoul(line, NULL, 10);
	pclose(cmd);

	//printf("PID: %d\n", pid_id);
	//printf("PIDs: %s\n", line);
	//printf("ptr: %s\n", strchr(line, ' '));

	if(strchr(line, ' ') != NULL) 
	{
		printf("Error: The program is already running!\n");
		exit(-2);
	}
	
	// Получение токена:
	key_t ipc_key = ftok(FILE_NAME, SHM_ID);

	if(ipc_key == -1){
		printf("ftok error: %s\n", strerror(errno));
		exit(-3);
	}

	//Создание SHM:
	while(1)
	{
		int shmem_id = shmget(ipc_key, 1*sizeof(data_struct), IPC_CREAT | 0666 );

		if(shmem_id == -1){
			printf("shmget error: %s\n", strerror(errno));
			exit(-4);
		}else{
			printf("PROGRAMM: \033[1;36m1\033[0m\n");
			
			int sem_id = semget(ipc_key, 1*sizeof(data_struct), IPC_CREAT | 0666);
			
			data_struct* ptr_to_shm = (data_struct*)shmat(shmem_id, NULL, 0);
			
			if(ptr_to_shm == (void*)(-1)){
				printf("SHMEM attach failed with code: %s\n", strerror(errno));
				exit(-5);
			}else if(sem_id == -1){
				printf("semget error: %s\n", strerror(errno));
				exit(-6);
			}else{
				semop(sem_id, &sem_open, 1);
				semop(sem_id, &sem_lock, 1); 
				// msops - кол. во едениц в массиве передачи данных.
				((data_struct*)ptr_to_shm)->ttime = time(0);
				((data_struct*)ptr_to_shm)->pid = getpid();

				semop(sem_id, &sem_open, 1);

				printf("Place in shm: \n [ttime: \033[31m%s\033[0m]\n "
					"[pid: \033[31m%d\033[0m]\n", 
					timeTostring(((data_struct*)ptr_to_shm)->ttime), 
					((data_struct*)ptr_to_shm)->pid);
		
				if(system("./Prog_2") == (-1))
				{
					printf("Error: ./Prog_2 alraydy running!\n");
				}
				
				sleep(4);

				semctl(sem_id, 0, IPC_RMID);// Помечаем на уданение.
				shmdt(ptr_to_shm);
				shmctl(shmem_id, IPC_RMID, NULL);
			}
		}
	}

	return 0;
}

