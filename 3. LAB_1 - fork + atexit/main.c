#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

void Parent_Atexit()
{
	printf("Сработал ataxit родительского процесса, наследуемый дочерним процессом!\n");
}

void Children_Atexit()
{
	printf("Сработал ataxit дочернего процесса!\n");
}

int main(int argc, char** argv)
{
	// fork();
	
	printf("Запущена процесс срабатывания функции: \x1b[36mfork()\x1b[0m\n");
	printf("Текущее состояние: \n");	
	printf("1. PID: \033[31m%3d\033[0m\n2. PPID: \033[31m%3d\033[0m\n", getpid(), getppid());

	atexit(Parent_Atexit);
	pid_t pid;
	int rv;
	switch(pid = fork())
	{
	case(-1):
		perror("fork error");
		exit(1);
		//break;
	case(0):
		atexit(Children_Atexit);
		printf("+ Это дочерний процесс!\n");
		printf("1.1 CHILD PID:  \033[31m%3d\033[0m\n", getpid());
		printf("1.2 PARENT PID: \033[31m%3d\033[0m\n", getppid());
		printf("1.3 Fork PID:   \033[31m%3d\033[0m\n", pid);

		printf("\033[32mУкажите код возврата для дочернего процесса:\033[0m ");
		scanf("%d",&rv);
		printf("\033[1;34mВыход из дочернего процесса!!!\033[0m\n");
		exit(rv);
		//break;
	default:
		printf("+ Это родительский процесс!\n");
		printf("2.1 Children PID: \033[31m%3d\033[0m\n", getpid());
		printf("2.2 Parent PID:   \033[31m%3d\033[0m\n", getppid());
		printf("2.3 fork PID:     \033[31m%3d\033[0m\n", pid);

		printf("Ожидание пока не завершит работу дочерний процесс!\n");
		wait(&rv);
		printf("PARENT: Код возврата потомка:\033[31m%3d\033[0m\n", WEXITSTATUS(rv));
		printf("Родительский процесс остается активным!\n");
		break;
	}


	printf("-------------------------------------------------------------------------\n");
	// vfork();
	printf("Запущена процесс срабатывания функции: \033[36mvfork()\033[0m\n");
	printf("Текущее состояние: \n");
	printf("1. PID: %1d\n2. PPID: %1d\n", getpid(), getppid());

	pid_t pid_2;
	int rv_2;
	switch(pid_2 = vfork())
	{
	case(-1):
		perror("vfork error");
		exit(1);
		//break;
	case(0):
		atexit(Children_Atexit);
		printf("+ Это дочерний процесс!\n");
		printf("1.1 CHILD PID:  \033[31m%3d\033[0m\n", getpid());
		printf("1.2 PARENT PID: \033[31m%3d\033[0m\n", getppid());
		printf("1.3 Fork PID:   \033[31m%3d\033[0m\n", pid_2);

		printf("\x1b[32mУкажите код возврата для дочернего процесса:\x1b[0m ");
		scanf("%d",&rv_2);
		printf("\033[1;34mВыход из дочернего процесса!!!\033[0m\n");
		exit(rv_2);
		//break;
	default:
		printf("+ Это родительский процесс!\n");
		printf("2.1 Children PID: \033[31m%3d\033[0m\n", getpid());
		printf("2.2 Parent PID:   \033[31m%3d\033[0m\n", getppid());
		printf("2.3 fork PID:     \033[31m%3d\033[0m\n", pid_2);
		wait(&rv_2);
		printf("Ожидание пока не завершит работу дочерний процесс!\n");
		printf("PARENT: Код возврата потомка: \033[31m%d\033[0m\n", WEXITSTATUS(rv_2));
		break;
	}


	printf("\033[1;34mЗавершение работы родительского процесса!\033[0m\n");
	return 0;
}
