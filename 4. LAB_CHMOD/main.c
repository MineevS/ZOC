#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h> // For Directories;
#include <getopt.h>

int main(int argc, char** argv)
{
	char c;
	char d;
	char e;
	char f;

	char directory[255] = "./";
	DIR* dir;
	dir = opendir(directory);
	int fd = dirfd(dir);
	struct stat file_st;

	while((c = getopt(argc, argv, "ugoa"))!= -1)
	{
		switch(c)
		{
		case('u'):
			
			while((d = getopt(argc, argv, "mp")) != -1)
			{
				switch(d)
				{
				case('m'):

					while((f = getopt(argc, argv, "rwx"))!= -1)
					{
						switch(f)
						{
						case('r'):
					
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IRUSR);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IRUSR);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IRUSR);
									}
								}
							}
						
							break;
						case('w'):
						
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IWUSR);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IWUSR);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IWUSR);
									}
								}
							}
							
							break;
						case('x'):
							
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IXUSR);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IXUSR);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IXUSR);
									}
								}
							}

							break;
						}
					}

					break;
				case('p'):
					
					while((f = getopt(argc, argv, "rwx"))!= -1)
					{
						switch(f)
						{
						case('r'):
					
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IRUSR);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IRUSR);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IRUSR);
									}
								}
							}
						
							break;
						case('w'):
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IWUSR);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IWUSR);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IWUSR);
									}
								}
							}
							
							break;
						case('x'):
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IXUSR);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IXUSR);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IXUSR);
									}
								}
							}


							break;
						}
					}

					break;
				}
			}

			break;
		case('g'):

			while((d = getopt(argc, argv, "mp")) != -1)
			{
				switch(d)
				{
				case('m'):

					while((f = getopt(argc, argv, "rwx"))!= -1)
					{
						switch(f)
						{
						case('r'):
					
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IRGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IRGRP);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IRGRP);
									}
								}
							}
						
							break;
						case('w'):
						
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IWGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IWGRP);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IWGRP);
									}
								}
							}
							
							break;
						case('x'):
							
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IXGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IXGRP);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IXGRP);
									}
								}
							}

							break;
						}
					}

					break;
				case('p'):
					
					while((f = getopt(argc, argv, "rwx"))!= -1)
					{
						switch(f)
						{
						case('r'):
					
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IRGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IRGRP);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IRGRP);
									}
								}
							}
						
							break;
						case('w'):
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IWGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IWGRP);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IWGRP);
									}
								}
							}
							
							break;
						case('x'):
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IXGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IXGRP);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IXGRP);
									}
								}
							}


							break;
						}
					}

					break;
				}
			}

			break;
		case('o'):

			while((d = getopt(argc, argv, "mp")) != -1)
			{
				switch(d)
				{
				case('m'):

					while((f = getopt(argc, argv, "rwx"))!= -1)
					{
						switch(f)
						{
						case('r'):
					
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IROTH);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IROTH);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IROTH);
									}
								}
							}
						
							break;
						case('w'):
						
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IWOTH);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IWOTH);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IWOTH);
									}
								}
							}
							
							break;
						case('x'):
							
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IXOTH);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IXOTH);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IXOTH);
									}
								}
							}

							break;
						}
					}

					break;
				case('p'):
					
					while((f = getopt(argc, argv, "rwx"))!= -1)
					{
						switch(f)
						{
						case('r'):
					
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IROTH);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IROTH);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IROTH);
									}
								}
							}
						
							break;
						case('w'):
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IWOTH);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IWOTH);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IWOTH);
									}
								}
							}
							
							break;
						case('x'):
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IXOTH);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IXOTH);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IXOTH);
									}
								}
							}


							break;
						}
					}

					break;
				}
			}

			break;
		case('a'):
		
			while((d = getopt(argc, argv, "mp")) != -1)
			{
				switch(d)
				{
				case('m'):

					while((f = getopt(argc, argv, "rwx"))!= -1)
					{
						switch(f)
						{
						case('r'):
					
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IROTH & ~S_IRUSR & ~S_IRGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IROTH & ~S_IRUSR & ~S_IRGRP);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IROTH & ~S_IRUSR & ~S_IRGRP);
									}
								}
							}
						
							break;
						case('w'):
						
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IWOTH & ~S_IWUSR & ~S_IWGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IWOTH & ~S_IWUSR & ~S_IWGRP);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IWOTH & ~S_IWUSR & ~S_IWGRP);
									}
								}
							}
							
							break;
						case('x'):
							
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode & ~S_IXOTH & ~S_IXUSR & ~S_IXGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode & ~S_IXOTH & ~S_IXUSR & ~S_IXGRP);
									}else{
										chmod(argv[i],  file_st.st_mode & ~S_IXOTH & ~S_IXUSR & ~S_IXGRP);
									}
								}
							}

							break;
						}
					}

					break;
				case('p'):
					
					while((f = getopt(argc, argv, "rwx"))!= -1)
					{
						switch(f)
						{
						case('r'):
					
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IROTH | S_IRUSR | S_IRGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IROTH | S_IRUSR | S_IRGRP);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IROTH | S_IRUSR | S_IRGRP);
									}
								}
							}
						
							break;
						case('w'):
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IWOTH | S_IWUSR | S_IWGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IWOTH | S_IWUSR | S_IWGRP);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IWOTH | S_IWUSR | S_IWGRP);
									}
								}
							}
							
							break;
						case('x'):
							for(size_t i = 1; i < argc; i++)
							{	
								if(stat(argv[i], &file_st) != -1)
								{
									if(S_ISDIR(file_st.st_mode))
									{// Обнаружена директория
										if((e = getopt(argc, argv, "R"))!= -1)
										{// Открытие директории и обслуживание рекурсивно!
											int dir_2;

											if((dir_2 = open(argv[i], O_RDONLY)) != 0)
											{

											}
										}
										else
										{//Иначе обработать директорию

											chmod(argv[i],  file_st.st_mode | S_IXOTH | S_IXUSR | S_IXGRP);
										}

									}else if(S_ISREG(file_st.st_mode))
									{// Обнаружен обычный файл
										chmod(argv[i],  file_st.st_mode | S_IXOTH | S_IXUSR | S_IXGRP);
									}else{
										chmod(argv[i],  file_st.st_mode | S_IXOTH | S_IXUSR | S_IXGRP);
									}
								}
							}

							break;
						}
					}

					break;
				}
			}

			break;
		default:
			break;
		}
	}

	return 0;
}
