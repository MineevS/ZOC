#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/wait.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define FIFO_NAME "temp_fifo"

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
       	dev_t dev;
	pid_t pid;       
	unlink(FIFO_NAME);

	int fifo = mkfifo(FIFO_NAME, S_IFIFO | 0777);

	if(fifo < 0)
	{
		printf("[CREATE] fifo creation error: %s\n", strerror(errno));
		return -1;
	}

	printf("PROGRAMM: \033[35mFIFO\033[0m; \n");
	
        switch(pid = fork())
        {
       	case(-1):
               printf("ERROR fork()\n");
               return -1;
       	case(0):
                printf("[CHILD] process\n");

		int fifo_read = fifo_read = open(FIFO_NAME, O_RDONLY);

                if(fifo_read < 0){
			printf("[READED] fifo creation errori: %s\n", strerror(errno));
			return -1;
		}
		
		data_struct data_rec;
                read(fifo_read, &data_rec, sizeof(data_struct));

		sleep(5);

		time_t st_t = time(0);
		char str_time[128] = "";
		strftime(str_time, 128, "%x %A %X %Z", localtime(&st_t));

		printf(" [CHILD] resived val: \n [time children: \033[32m%s\033[0m] \n [time parent: \033[32m%s\033[0m] \n [pid: \033[31m%d\033[0m]\n", str_time, timeTostring(data_rec.ttime), data_rec.pid);
		
                return 0;
        default:
                printf("[PARENT] process\n");

                data_struct data_send;
		data_send.ttime = time(0);
		data_send.pid = getpid();
		
		int fifo_write = open(FIFO_NAME, O_WRONLY);

		if(fifo_write < 0){
			 printf("[READED] fifo creation error: %s\n",strerror(errno));
		}

                write(fifo_write, (void*)(&data_send), sizeof(data_struct));
                
		printf("[PARENT] sended val: \n [time: \033[32m%s\033[0m] \n [pid: \033[31m%d\033[0m]\n", timeTostring(data_send.ttime), data_send.pid);
                
		wait(0);
                break;
        }

        return 0;
}

