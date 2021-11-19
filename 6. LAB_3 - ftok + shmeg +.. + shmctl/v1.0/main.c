#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define FILE_NAME "main.c"
#define SHM_ID 1

// Тема: Разделяемая память[Shared memory];

int main()
{
	// Получение токена:
	key_t ipc_key = ftok(FILE_NAME, SHM_ID);

	if(ipc_key == -1){
		perror("ftok error!");
		exit(-1);
	}

	//Создание SHM:
	size_t shmid_1 = shmget(ipc_key, 1000, IPC_CREATE | 0666 );

	if(shmid_1 == -1){
		perror("shmget error!");
		exit(-2);
	}else{
		printf("[SHMID_1 = %d\n]", shmid_1);
	}

	size_t shmid_2 = shmget(ipc_key, 1000, IPC_CREATE | IPC_EXCL  | 0666 );

        if(shmid_2 == -1){
                perror("shmget error!");
                exit(-2);
        }else{
                printf("[SHMID_2 = %d\n]", shmid_2);
        }

	return 0;
}

// + Высокая скорость чтения и записи.
// + Произвольный доступ.
// + Не требуется синхронизации.
// + 
