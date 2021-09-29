#include <dirent.h> // For Directories;
#include <stdio.h>  // For I/O;
#include <string.h>
#include <stdlib.h> // For exit();
#include <unistd.h> // getopt();
#include <getopt.h> // getopt_long();
#include <sys/stat.h> // For param files;
#include <string.h>// strstok();
#include <time.h> // Forasctime
#include <sys/types.h> //
#include <pwd.h>
#include <grp.h>

int strmode(mode_t mode, char* buf);
int parse_to_int(nlink_t *source);
void file_seek(FILE* file,int num_str, int stat);

char *strrev(char *str)
{
      char *p1, *p2;
      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

int main(int argc, char** argv)
{
	char c; 	// Возврашение результата getopt();
	int error = 0; 	// увеличивается, когда возникают неизвестная аргументная строка.
	DIR* dir;
	struct dirent* entry;
	char directory[255] = "./";
	struct stat file_st;
        int option_index = 0;
        static struct option long_options[] = {
            {"add", required_argument, 0, 0},
            {"append", 0, 0, 0},
            {"delete", required_argument, 0, 0},
            {"verbose", 0, 0, 0},
            {"create", required_argument, 0, 'c'},
            {"file", required_argument, 0, 0},
	    {"help", no_argument, 0, 0},
            {0, 0, 0, 0}
        };
	
	if((c = getopt_long(argc, argv, "abc:d:012", long_options, &option_index)) != -1)
	{
		if(option_index != 0) printf("%s \n", long_options[option_index].name);
		if(option_index == 0) printf("\033[1;31mПараметр\033[0m: %c;\n\033[1;31mАргумент\033[0m: %s;\n", c, optarg);
		
	}
	else if((c = getopt(argc, argv, "a:b")) == -1)
	{
		if(argc > 1)
		{
			printf("\x1b[1;32mЗапущен процесс создания или открытия архива!\x1b[0m\n");
			
			// [1]: Требуется проверить есть ли файл: argv[1] - ?
			
			//// Проверка наличия файла в директории:
			dir = opendir(directory);
			if(!dir) exit(-1);

			for(size_t i = 1; i < argc; i++)// Цикал аргументных имен архивов; 
			{
				// i ~ index_arcg_file;

				while((entry = readdir(dir)) != NULL)
				{
					if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
				
					char file_name[strlen(entry->d_name)];
					memset(file_name, 0, strlen(entry->d_name) + 1);
					strncpy(file_name, entry->d_name, strlen(entry->d_name));

					if(strcmp(file_name, argv[i]) || strcmp(file_name, strncat(argv[i], ".kmb", 5)))
					{
						FILE *file_arch;
						int fd = dirfd(dir);
						struct stat file_arch_st;

						/*if(strncmp(strrev(strdup(argv[i])), strrev(strdup(".kmb")), 4) == 0)
						{
							if((file_arch = fopen(argv[i], "a+")) != NULL)
							{
								printf("12 \n");
								fstatat(fd, argv[i], &file_st, 0);
							}
						}else{
							if((file_arch = fopen(strncat(argv[i], ".kmb", 5), "a+")) != NULL)
							{
								printf("13 \n");
								fstatat(fd, argv[i], &file_st, 0);

							}
						}*/

						if((file_arch = fopen(file_name, "w+")) == 0)
						{
							//printf(" \n");
							fstatat(fd, file_name, &file_arch_st, 0);
							
							// TODO;
							
							for(size_t j = 1; j < argc; j++)// Цикл аргументных имен файлов
							{
								// TODO;
								// argv[j]

								if(j == i) continue;//  т. к. argv[i] - имя arch_file

								FILE* file;

								if((file = fopen(argv[j], "r")) == 0)
								{
									file_seek(file_arch, 1, 1);
									fprintf(file_arch, "%s||", argv[j]);
								}
								else
								{
								}

								fclose(file);
							}
						}
						
						fclose(file_arch);
					}
					else
					{
						for(size_t k = 1; k < argc; k++){

						printf("Не обнаружен arch_file:\n Создать arch_file c именем: %s",argv[k]);
						switch(getchar())
						{
						case('Y'):
							printf("\033[1;33m::Y::\033[0m");

							// TODO;

							break;
						default:
							break;
						};

						
						}
					}
				}
			}
		} 
		else
		{
			printf("2: \n");
			
			dir = opendir(directory);

			if(!dir)
			{
				perror("diropen");
				exit(1);
			}

			while((entry = readdir(dir)) != NULL)
			{
				if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
				
				//printf("%d - %s [%d] %d \n", entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
				char file_name[strlen(entry->d_name)];
				//strcpy(file_name, directory);
				//if(strcmp(file_name, "\\")) strcat(file_name,"\\" );
				//strcat(file_name, entry->d_name);
				strncpy(file_name, entry->d_name, strlen(entry->d_name));
				strncpy(file_name, file_name,strcspn(file_name, "."));
				//stat(file_name,&file_st);

				if((stat(file_name, &file_st)) != -1)
				{
					char mode[10];
					strmode(file_st.st_mode, mode);
					struct passwd* pwd;
					pwd = getpwuid(file_st.st_uid);
					if(pwd == NULL) continue;
					struct group*  gro;
					gro = getgrgid(file_st.st_gid);
					
					struct tm* timeinfo = localtime(&file_st.st_mtime);

					printf("-%s  %d  %s  %8s", mode, file_st.st_nlink, pwd->pw_name, gro->gr_name);
					char buf_time[255];
					strftime(buf_time, sizeof(buf_time), "%d %b %Y %H:%M", timeinfo);
					
					if(file_st.st_mode & S_IXUSR)
					{
					   printf(" %6ld  %2s\x1b[0;32;40m %-8s \x1b[0m \n", file_st.st_size, buf_time, file_name );
					}else{
						printf(" %6ld  %2s  %-8s  \n", file_st.st_size, buf_time, file_name );
					}					
				}
			}

			closedir(dir);
		}
	}
	else
	{
		do{
			switch(c)
			{
			case('a'):
				printf("Arg is: %s ; \n", optarg);
				break;
			case('b'):
				printf("FLAG b recieved  \n");
				break;
			case('?'):
				error += 1;
				printf("Uncnown FLAG \n");
				break;
			default:
				printf("Uncnown FLAG \n");
				dir = opendir(directory);

				if(!dir)
				{
					perror("diropen");
					exit(1);
				}

				while((entry = readdir(dir)) != NULL)
				{
					printf("%d - %s [%d] %d \n", entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
				}

				closedir(dir);
				break;
			}

		}while((c = getopt(argc, argv, "a:b")) != -1);
	}

	return 0;
}


int strmode(mode_t mode, char* buf)
{
	const char chars[] = "rwxrwxrwx";

	for(size_t i = 0; i<9; i++)
	{
		buf[i] = (mode & (1 << (8 - i))) ? chars[i] : '-';
	}
	buf[9] = '\0';

}

int parse_to_int(nlink_t *source)
{
  int buffer_size = sizeof(*source);
  char buffer[buffer_size];
  snprintf(&buffer[0], buffer_size, "%lu", (unsigned long)source);
  int val = atoi(buffer);
  return val;
}

void file_seek(FILE* file, int num_str, int stat)
{

	if(stat == 1)
	{
		for(size_t i = 0; i < num_str; i++)
		{
			while((fgetc(file) != '\n') || (feof(file) == EOF))
			{

			}

			fseek(file, ftell(file) - 1, SEEK_SET);
		}
	}
}

