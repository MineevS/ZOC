#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/wait.h>

typedef struct data_struct
{
        time_t ttime;
        pid_t pid;

}data_struct;

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
       int pipe_des[2];
        pid_t pid;
        pipe(pipe_des);

	printf("PROGRAMM: \033[35mPIPE\033[0m; \n");
	
        switch(pid = fork())
        {
       	case(-1):
               printf("ERROR fork()\n");
               return -1;
       	case(0):
                printf("[CHILD] process\n");
                close(pipe_des[1]);
		
		data_struct data_rec;
                read(pipe_des[0], &data_rec, sizeof(data_struct));

		sleep(5);

		time_t st_t = time(0);
		char str_time[128] = "";
		strftime(str_time, 128, "%x %A %X %Z", localtime(&st_t));

		printf(" [CHILD] resived val: \n [time children: \033[32m%s\033[0m] \n "
			"[time parent: \033[32m%s\033[0m] \n [pid: \033[31m%d\033[0m]\n", 
			str_time, timeTostring(data_rec.ttime), data_rec.pid);
		
		close(pipe_des[0]);
                return 0;
        default:
                printf("[PARENT] process\n");
		close(pipe_des[0]);

                data_struct data_send;
		data_send.ttime = time(0);
		data_send.pid = getpid();
		
                write(pipe_des[1], (void*)(&data_send), sizeof(data_struct));
                
		printf("[PARENT] sended val: \n [time: \033[32m%s\033[0m] \n [pid: \033[31m%d\033[0m]\n", 
			timeTostring(data_send.ttime), data_send.pid);
                
		close(pipe_des[1]);
		wait(0);
                break;
        }

        return 0;
}

