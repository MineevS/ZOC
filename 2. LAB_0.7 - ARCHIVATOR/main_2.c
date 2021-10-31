#include <dirent.h> // For Directoriesb64/libarchive.so;
#include <stdio.h>  // For I/O;
#include <string.h>
#include <stdlib.h> // For exit();
#include <unistd.h> // getopt();
#include <getopt.h> // getopt_long();
#include <sys/stat.h> // For param files, chmod();
#include <string.h>// strstok();
#include <time.h> // Forasctime
#include <sys/types.h> //
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <sys/io.h>
//#include <io.h> // FOR SEEK_SET, SEEK_END;

void func_input_in_arch(int argc,char** argv,char* directory, DIR* dir,struct dirent* entry, struct stat file_st);
void func_write_in_arch(int argc,char** argv,int i, int fd_file_arch, struct stat file_st, size_t count_files);
void fmode(mode_t mode, char* buf);

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

char* strmemcp(char* str)
{
	char* mem = malloc(strlen(str)*sizeof(char));
	memset(mem, 0, (strlen(str) + 1)*sizeof(char));
	strncpy(mem, str, strlen(str));

	return mem;
}

int args_strcmp(const char* buff, char** argv, int argc)
{
	int flag = 1;
	for(size_t i = 0; i < argc; i++)
	{
		if(strcmp(buff, argv[i]) == 0)
		{
			flag = 0;
		}
	}

	return flag;
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
            {"input", no_argument, 0, 'i'},//0
            {"extract", no_argument, 0, 'e'},//1
            {"stat", no_argument, 0, 's'},//2
	    {"help", no_argument, 0, 'h'},//3
	    {"delete",no_argument,0, 'd'},// 4
        {0, 0, 0, 0}
        };
	
	if((c = getopt_long_only(argc, argv, "abc012", long_options, &option_index)) != -1)
	{
		//if(option_index != 0) printf("%s - %d \n", long_options[option_index].name, option_index);
		//if(option_index == 0) printf("\033[1;31mПараметр\033[0m: %c;\n\033[1;31mАргумент\033[0m: %s;\n", c, optarg);
		
		switch(option_index)
		{
		case(0):
			printf("input file(s)\n");
			
			func_input_in_arch(argc, argv, directory, dir, entry, file_st);
			break;
		case(1):
			printf("extract file(s)\n");

			if(argc > 1)
			{
				printf("Запущен процесс извлечения файлов из архива.\n");
				
				int f = 0;

				for(size_t i = 0; i < argc; i++)//Цикал аргументных имен архивов;
				{
					dir = opendir(directory);
					if(!dir) exit(1);
				
					while((entry = readdir(dir)) != NULL)
					{
						 if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

						char file_name[strlen(entry->d_name)];
						memset(file_name, 0, strlen(entry->d_name) + 1);
						strncpy(file_name, entry->d_name, strlen(entry->d_name));

						if(!strcmp(file_name, argv[i]) || !strcmp(file_name, strncat(strmemcp(argv[i]), ".kmb", 5)))
						{
							int fd_file_arch;
							struct stat file_arch_st;

							if(strncmp(strrev(strmemcp(file_name)), strrev(strmemcp(".kmb")), 4) != 0) continue;
							
							if((fd_file_arch = open(file_name, O_RDONLY)) != 0)
							{
								f = 1;
								stat(file_name,&file_arch_st);
								
								if(file_arch_st.st_size == 0)
								{
									//printf("Файл пуст!\n");
								}
								else
								{
									//printf("Файл не пуст!\n");
									
									for(size_t j = 3; j < argc; j++)
									{
										int fl = 1;
										
										if(((stat(argv[j], &file_st))!= -1) && (strncmp(argv[j],"-",1) != 0) && strcmp(argv[i],argv[j]) != 0)
										{
											printf("Файл с именем \033[1;32m%s\033[0m уже существует в той директории, где находится и сам архив!\n", argv[j]);

											printf("Если в архиве найдется файл с таким же именем, то перезаписать уже существующий файл? (Y,y /N): ");

											switch(getchar())
											{
											case('Y'):
												printf("\nЕсли в архиве найдется файл с таким же именем, то Файл будет перезаписан!\n");
												break;
											case('y'):
												printf("\nЕсли в архиве найдется файл с таким же именем, то Файл будет перезаписан!\n");
												break;
											default:
												printf("Файл \033[1;32m%s\033[0m - не извлечен!\n", argv[j]);
												continue;
											}
										}

										size_t a = 0; long pos = 0;                          
										lseek(fd_file_arch,pos,SEEK_SET);   
		 								// Читать счетчик
										read(fd_file_arch, (void*)&a, sizeof(size_t)*1);

										////////////////////////////////////////////
										size_t len_name_file = 0;
										char* buff = (char *)malloc(sizeof(char)*len_name_file);
										size_t b, c, d;
										size_t size_f_1 = 0;
										size_t size_f_2 = 0;
										
										size_t fb = 0;
										mode_t fmode = 0;
											
										for(size_t w = 0; w < a; w++)
										{
											read(fd_file_arch,&len_name_file, sizeof(size_t)*1);
											memset(buff, 0, len_name_file + 1);
											read(fd_file_arch, buff, sizeof(char)*len_name_file);//Получение имени файла	
											read(fd_file_arch,( void*)&b, sizeof(size_t)*1);// Получение размера файла
											
											if(fl){
												if((strcmp(argv[j], buff)) == 0){fl = 0;}
												else{size_f_1 += b;}
											}
											
											if((strcmp(argv[j], buff)) == 0){fb = b;}
											size_f_2 += b;

											mode_t mt;//Получение режима файла.
											if((strcmp(argv[j], buff)) == 0)
											{
												read(fd_file_arch, &fmode, sizeof(mode_t));// Получение режима файла
											}else{
												read(fd_file_arch, &mt, sizeof(mode_t));
											}
											
											c = lseek(fd_file_arch, 0, SEEK_CUR);// Текущее положение курсора
											lseek(fd_file_arch, 0, SEEK_END);
											d = lseek(fd_file_arch, 0, SEEK_CUR);// Размер всего файла
											lseek(fd_file_arch, c, SEEK_SET);
										}

										if((((d - c) == size_f_2) && (!fl)))
										{
											printf("Создание файла: %s\n", argv[j]);
											
											lseek(fd_file_arch, c + size_f_1, SEEK_SET);
											// TODO;
											int size_mas = fb;
											char * mas = (char*)malloc(size_mas*sizeof(char));
											memset(mas,0,sizeof(char)*size_mas + 1);
											read(fd_file_arch, mas, sizeof(char)*fb);
											
											if(strstr(argv[0], argv[j]) != NULL) continue;// argv[0]="./gdb", argv[j]="gdb"
											
											int file_f;
											if((file_f = open(argv[j], O_RDWR | O_CREAT| O_TRUNC, S_IRUSR| S_IWUSR| S_IRGRP))!= 0)
											{
												write(file_f, mas, sizeof(char)*fb);
												
												chmod(argv[j], fmode);
											}
											
											close(file_f);
										}else{
											
											printf("Файл с именем в архиве не обнаружен! \n");
										}
									}
								}
								close(fd_file_arch);
							}
						}
					}
					
					closedir(dir);
				}	
			}

			break;
		case(2):
			printf("stat archive: \n");
		
			if(argc > 1)
			{
				int f = 0;

				for(size_t i = 0; i < argc; i++)//Цикал аргументных имен архивов;
				{
					dir = opendir(directory);
					if(!dir) exit(1);
				
					while((entry = readdir(dir)) != NULL)
					{
						if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

						char file_name[strlen(entry->d_name)];
						memset(file_name, 0, strlen(entry->d_name) + 1);
						strncpy(file_name, entry->d_name, strlen(entry->d_name));

						if(!strcmp(file_name, argv[i]) || !strcmp(file_name, strncat(strmemcp(argv[i]), ".kmb", 5)))
						{
							printf("Обнаружен архив: \033[1;33m%s\033[0m \n", argv[i]);
							int fd_file_arch;
							struct stat file_arch_st;

							if(strncmp(strrev(strmemcp(file_name)), strrev(strmemcp(".kmb")), 4) != 0) continue;
							if((fd_file_arch = open(file_name, O_RDONLY)) != 0)
							{
								f = 1;
								stat(file_name, &file_arch_st);
								
								if(file_arch_st.st_size == 0)
								{
									printf("Архив пуст!\n");
								}
								else
								{
									//printf("Файл не пуст!\n");
									
									size_t a = 0;                         
									lseek(fd_file_arch,0,SEEK_SET);   
		 							// Читать счетчик
									read(fd_file_arch,(void*)&a, sizeof(size_t));

									////////////////////////////////////////////
									size_t len_name_file = 0;
									char* buff = (char *)malloc(sizeof(char)*len_name_file);
									size_t b, c, d;
									size_t size_f_1 = 0;
									size_t size_f_2 = 0;
									
									printf("Обнажено %zu файлов в архиве: \n", a);
									
									for(size_t w = 1; w <= a; w++)
									{
										read(fd_file_arch, &len_name_file, sizeof(size_t));
										memset(buff, 0, len_name_file + 1);
										read(fd_file_arch, buff, sizeof(char)*len_name_file);//Получение имени файла
										
										read(fd_file_arch, (void*)&b, sizeof(size_t));// Получение размера файла
			
										size_f_2 += b;

										mode_t mt;//Получение режима файла.
										read(fd_file_arch, &mt, sizeof(mode_t));
										
										char buffm[10];
										memset(buffm, 0, 10);
										fmode(mt, buffm);
										
										printf("\033[1;31m%zu\033[0m.%s size:\033[1;36m%zu\033[0m  mode:\033[1;34m%s\033[0m\n", w, buff, b, buffm);
									}
								}

								close(fd_file_arch);
							}
						}
					}
					
					closedir(dir);
				}	
			}
		
			break;
		case(3):
			printf("help man:\n");
			
			printf("1. Добавление файлов в архив: -input file_1 ..., --input file_1 ...\n");//+
			printf("2. Изьятия файлов из архива: -extract file_1 ..., --extract file_1 ...\n");//+
			printf("3. Удаление файлов из архива: -delete file_1 ..., --delete file_1 ...\n");//-
			
			
			printf("ВАЖНО: В случае \033[1;31mпервичного\033[0m добавления файлов в архив(т. е. создание архива и добавление файлов в созданный архив.) \n в аргументах командной строки должено быть указано имя архив - файла с типом \033[1;32m\".kmb\"\033[0m явно!\n");
			printf("ВАЖНО: В случае уже существующего архива для добавления файлов в архив можно не указывать \033[1;32m\".kmb\"\033[0m в аргументах!\n");
			printf("По умолчанию вызов команды с архивом и файлами автоматически добавляет файлы в архив!\n");
			break;
		case(4):
			

			if(argc > 1)
			{
				//printf("????o? ???y ??????.\n");
				
				int f = 0;

				for(size_t i = 0; i < argc; i++)//?? ????? ?????
				{
					dir = opendir(directory);
					if(!dir) exit(1);
				
					while((entry = readdir(dir)) != NULL)
					{
						 if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

						char file_name[strlen(entry->d_name)];
						memset(file_name, 0, strlen(entry->d_name) + 1);
						strncpy(file_name, entry->d_name, strlen(entry->d_name));

						if(!strcmp(file_name, argv[i]) || !strcmp(file_name, strncat(strmemcp(argv[i]), ".kmb", 5)))
						{
							int fd_file_arch;
							struct stat file_arch_st;

							if(strncmp(strrev(strmemcp(file_name)), strrev(strmemcp(".kmb")), 4) != 0) continue;

							if((fd_file_arch = open(file_name, O_RDONLY)) != 0)
							{
								f = 1;
								stat(file_name, &file_arch_st);
								
								if(file_arch_st.st_size == 0)
								{
									//printf("?????");
								}
								else
								{
									//printf("??????");
									int flg = 0;
									
									for(size_t j = 0; j < argc; j++)
									{
										int fl = 1;
										
										if((stat(argv[j], &file_st))!= 0)
										{
											//printf("??????s ?????????????? ????????\n", argv[j]);
										}

										size_t a = 0;                          
										lseek(fd_file_arch,0,SEEK_SET);   
										read(fd_file_arch,(void*)&a, sizeof(size_t));

										////////////////////////////////////////////
										size_t* len_name_file = (size_t*)malloc(a*sizeof(size_t));
										size_t* fb = (size_t*)malloc(a*sizeof(size_t));

										char** buff = (char **)malloc(a*sizeof(char*));
										size_t b, c, d;
										size_t size_f_1 = 0;
										size_t size_f_2 = 0;
										
										//size_t fb = 0;
										mode_t* fmode = (mode_t*)malloc(a*sizeof(mode_t));
										size_t fw = 0;

										for(size_t w = 0; w < a; w++)
										{	
											read(fd_file_arch, &len_name_file[w], sizeof(size_t));
											
											buff[w] =(char*)malloc(len_name_file[w]*sizeof(char));
											memset(buff[w], 0, len_name_file[w] + 1);
											read(fd_file_arch, buff[w], sizeof(char)*len_name_file[w]);//????????
											//fb[w] = (size_t) realloc(fb, sizeof(size_t));
											read(fd_file_arch, &fb[w], sizeof(size_t));// ?????? ???										
											//fmode[w] = (mode_t) realloc(fmode, sizeof(mode_t));
											read(fd_file_arch, &fmode[w], sizeof(mode_t));// ?????????										
											c = lseek(fd_file_arch, 0, SEEK_CUR);// ?????? ???
											lseek(fd_file_arch, 0, SEEK_END);
											d = lseek(fd_file_arch, 0, SEEK_CUR);// Р???? ???
											lseek(fd_file_arch, c, SEEK_SET);
										}

										if(a == 0)continue;

										char* mas;
										mas = (char*)malloc(sizeof(char)*(d - c));
										memset(mas, 0, (d - c) + 1);
										read(fd_file_arch, mas, sizeof(char)*(d - c));
										
										close(fd_file_arch);

										if((fd_file_arch = open(file_name, O_RDWR| O_TRUNC)) != 0)
										{
											write(fd_file_arch, &a, sizeof(size_t));
											for(size_t w = 0; w < a; w++)
											{
												if(args_strcmp(buff[w], argv, argc) != 0)
												{
													write(fd_file_arch, &len_name_file[w], sizeof(size_t));
													write(fd_file_arch, buff[w], sizeof(char)*len_name_file[w]);
													write(fd_file_arch, &fb[w], sizeof(size_t));
													write(fd_file_arch, &fmode[w], sizeof(mode_t));

													if(fl){
														size_f_1 +=fb[w];
													}else{
														size_f_2 +=fb[w];
													}
												}
												else if(args_strcmp(buff[w], argv, argc) == 0)
												{
													flg = w;
													fl = 0;
													size_t b_s = a-1;
													size_t c_ = lseek(fd_file_arch, 0, SEEK_CUR);
													lseek(fd_file_arch,0, SEEK_SET);
													write(fd_file_arch, &b_s, sizeof(size_t));
													fsync(fd_file_arch);
													lseek(fd_file_arch,c_, SEEK_SET);
													
													if(w){
														printf("delete file %s!\n", buff[w]);
													}
													
												}
											}
											
											
											
											write(fd_file_arch, mas, sizeof(char)*size_f_1);
											write(fd_file_arch, mas+(size_f_1+fb[flg])*sizeof(char), sizeof(char)*size_f_2);
											fsync(fd_file_arch);
										}
									}
									
									if(!flg){
										printf("В архиве удаляемого файла не обнаружено!\n");
									}
								}

								close(fd_file_arch);
							}
						}
					}
					
					closedir(dir);
				}	
			}

			break;
		default:

			break;
		};
	
	}
	else if((c = getopt(argc, argv, "a:b")) == -1)// Если отсутствуют флаги.
	{
		func_input_in_arch(argc, argv, directory, dir, entry, file_st);
	}
	
	return 0;
}

void func_input_in_arch(int argc,char** argv,char* directory, DIR* dir,struct dirent* entry, struct stat file_st)
{
	if(argc > 1)
	{
		//// Проверка наличия файла в директории:
		dir = opendir(directory);
		if(!dir) exit(-1);
		int f = 0;
		int num = 0;
		
		for(size_t i = 1; i < argc; i++)// Цикал аргументных имен архивов; 
		{
			// i ~ index_argc_file;
			while((entry = readdir(dir)) != NULL)
			{
				if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
				
				char file_name[strlen(entry->d_name)];
				memset(file_name, 0, strlen(entry->d_name) + 1);
				strncpy(file_name, entry->d_name, strlen(entry->d_name));

				if(!strcmp(file_name, argv[i]) || !strcmp(file_name, strncat(strmemcp(argv[i]), ".kmb", 5))) 
				{
					int fd_file_arch;
					struct stat file_arch_st;
					int count_files = 0;

					if(strncmp(strrev(strmemcp(file_name)), strrev(strmemcp(".kmb")), 4) != 0) continue;

					if((fd_file_arch = open(file_name, O_RDWR)) != 0)
					{
						f = 1;
						stat(file_name, &file_arch_st);

						if(file_arch_st.st_size == 0)
						{
							//printf("Файл пуст!\n");
							func_write_in_arch(argc, argv, i, fd_file_arch, file_st, 0);
						}
						else
						{
							//printf("Файл не пуст!\n");
							//Обновить счетчик в двоичном файле
							size_t a = 0; long pos = 0;                          
							lseek(fd_file_arch,pos,SEEK_SET);   
		 					// Читать счетчик
							read(fd_file_arch,(void*)&a, sizeof(size_t));

							func_write_in_arch(argc, argv, i, fd_file_arch, file_st, a);
						}
					}
					
					close(fd_file_arch);
					
				}
			}

			if(!f){// Если архив не обнаружен!	

				if(strncmp(strrev(strmemcp(argv[i])),strrev(strmemcp(".kmb")), 4) != 0){num +=1; continue;}
				
				printf("\x1b[1;32mЗапущен процесс создания архива!\x1b[0m\n");
				
				printf("Архив с именем \033[35m%s\033[0m не обнаружен в данной директории\n", argv[i]);
				printf("Создать архив с именем \033[32m%s\033[0m: ", argv[i]);

				switch(getchar())
				{
				case('Y'):
					printf("Y \n");
					break;
				case('y'):
					printf("y \n");

					int fd_file_arch;

					if((fd_file_arch = open(argv[i], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR )) != 0)// Работа с архиватором;//|S_IRGRP|S_IWGRP|S_IROTH|S_IXOTH
					{
						func_write_in_arch(argc, argv, i, fd_file_arch, file_st, 0);
					}
					
					close(fd_file_arch);
					break;
				default:
					break;
				}
			}
		}
		
		if(num == argc-1){
			printf("В командной строке в качестве аргумента не обнаружен файл с типом \".kmb\".\n");
			printf("В рабочей директории необнаружен файл %s.kmb\".\n", argv[1]);
			printf("В случае \033[1;31mпервичного\033[0m добавления файлов в архив(т. е. создание архива и добавление файлов в созданный архив.) в аргументах командной строки должено быть указано имя архив - файла с типом \033[1;32m\".kmb\"\033[0m явно!\n");
			printf("В случае уже существующего архива для добавления файлов в архив можно не указывать \033[1;32m\".kmb\"\033[0m в аргументах!");
			printf("Работа программы прекращена!\n");
		}
	}
}

void func_write_in_arch(int argc,char** argv,int i, int fd_file_arch, struct stat file_st, size_t count_files)
{
	//size_t count_files = 0;
	for(size_t j = 1; j < argc; j++)// Цикл аргументных имен файлов
	{
		// TODO;

		if(j == i) continue;//  т. к. argv[i] - имя arch_file

		int file;
		
		if(((file = open(argv[j], O_RDONLY)) != 0) && (count_files == 0))
		{
			if(stat(argv[j], &file_st) !=0 )continue;
			
			printf("Успешно был открыт файл: \033[1;31m%s \033[0m\n", argv[j]);
			char* buff_file = (char*)malloc(file_st.st_size * sizeof(char));
			read(file, buff_file, sizeof(char)*file_st.st_size);
			count_files++;
			write(fd_file_arch, &count_files, sizeof(size_t));
			size_t s = strlen(argv[j]);
			write(fd_file_arch, &s, sizeof(size_t));
			write(fd_file_arch, argv[j], sizeof(char)*strlen(argv[j]));
			write(fd_file_arch, &file_st.st_size, sizeof(file_st.st_size));
			write(fd_file_arch, &file_st.st_mode, sizeof(file_st.st_mode));
			write(fd_file_arch, buff_file, sizeof(char)*file_st.st_size);
			fsync(fd_file_arch);

			close(file);
		}
		else if(((file = open(argv[j], O_RDONLY)) != 0)&&(count_files > 0))
		{
			if (strncmp(argv[j], "-",1) == 0) continue;
			
			if(stat(argv[j], &file_st) != 0){
				printf("При попытке добавление файла %s в архив было обнаружено, что файла не существует в рабочей директории!\n", argv[j]);
				printf("Файл %s \033[1;33mНе добавлен в архив\033[0m!\n", argv[j]);
				continue;
			};

			printf("Успешно был открыт файл: \033[1;31m%s \033[0m\n", argv[j]);
			char* buff_file = (char*)malloc(file_st.st_size * sizeof(char));
			read(file, buff_file, sizeof(char)*file_st.st_size);
			count_files++;

			//Обновить счетчик в двоичном файле
			size_t a = 0; long pos = 0;                          
			lseek(fd_file_arch,pos,SEEK_SET);   
		 	// Читать счетчик
			read(fd_file_arch,(void*)&a, sizeof(size_t));
			a++; // Увеличить в памяти
			lseek(fd_file_arch,pos,SEEK_SET);   // Записать обратно по тому же адресу
			write(fd_file_arch,(void*)&a, sizeof(size_t));

			////////////////////////////////////////////
			char* buff = (char *)malloc(sizeof(char));
			memset(buff, 0, sizeof(char) + 1);
			char** files_mas = (char**)calloc(0, sizeof(char*));
			size_t b, c, d;
			size_t size_f = 0;
			char buff_new_filename[256] = {0};
			for(size_t w = 1; w < a; w++)
			{
				size_t len_name_file = 0;
				read(fd_file_arch, &len_name_file, sizeof(size_t));
				read(fd_file_arch, buff, sizeof(char)*len_name_file);

				if(strcmp(buff, argv[j]) == 0)
				{
					printf("В архиве обнаружен файл с похожим именем %s\n", argv[j]);
					printf("Введите новое имя файла для возможности сохранения файла в архивс учетом типа файла!: ");
					scanf("%s", buff_new_filename);
					while(strlen(buff_new_filename) > 256 || (strncmp(argv[j], buff_new_filename, strlen(buff_new_filename)) == 0))
					{
						if(strncmp(argv[j], buff_new_filename, strlen(buff_new_filename)) == 0)
						{
							printf("Новое имя повторяется с именем уже находящегося файла в архиве. Введите имя неповторяющееся с тем именем файла, что есть в архиве!\n");
						}if(strlen(buff_new_filename) == 0){
							printf("Нулевая длина нового имени записываемого файла воспринята как отмена операции добавления файла в архив.\n");
							printf("Программа заверщает работу!\n");
							exit(0);
						}
						
						printf("Введите новое имя файла для возможности сохранения файла в архив c не более 255 символами и неповторяющимся именем c учетом типа файла!: ");
						scanf("%s", buff_new_filename);
					}

					printf("Данные учтены!\n");
				}

				read(fd_file_arch, (void*)&b, sizeof(size_t));
				size_f += b;

				mode_t mt;//Получение режима файла.
				read(fd_file_arch, &mt, sizeof(mode_t));
								
				c = lseek(fd_file_arch, 0, SEEK_CUR);// Текущее положение курсора

				lseek(fd_file_arch, 0, SEEK_END);

				d = lseek(fd_file_arch, 0, SEEK_CUR);// Размер всего файла
					
				lseek(fd_file_arch, c, SEEK_SET);
			}

			if((d - c) == size_f)
			{
				lseek(fd_file_arch, c, SEEK_SET);
				int cs = lseek(fd_file_arch, 0, SEEK_CUR);
				int size_mas = d - cs;
				char * mas = (char*)malloc(size_mas*sizeof(char));
				read(fd_file_arch, mas, sizeof(char)*size_mas);
				lseek(fd_file_arch,cs, SEEK_SET);
				size_t st = 0;

				if(strlen(buff_new_filename) >0)
				{
					st = strlen(buff_new_filename);
					write(fd_file_arch, &st,sizeof(size_t));
					write(fd_file_arch, buff_new_filename, sizeof(char)*st);

				}
				else
				{
					st = strlen(argv[j]);
					write(fd_file_arch, &st, sizeof(size_t));
					write(fd_file_arch, argv[j], sizeof(char)*st);
				}

				write(fd_file_arch, &file_st.st_size, sizeof(file_st.st_size));					
				write(fd_file_arch, &file_st.st_mode, sizeof(mode_t));
				write(fd_file_arch, mas, sizeof(char)*size_mas);

				write(fd_file_arch, buff_file, sizeof(char)*file_st.st_size);
				fsync(fd_file_arch);
			}

			close(file);
		}else{
			printf("При попытке добавление файла %s в архив было обнаружено, что файла не существует в рабочей директории!\n", argv[j]);
			printf("Файл %s \033[1;33mНе добавлен в архив\033[0m!\n", argv[j]);
		}
	}
}

void fmode(mode_t mode, char* buf)
{
	const char chars[] = "rwxrwxrwx";

	for(size_t i = 0; i<9; i++)
	{
		buf[i] = (mode & (1 << (8 - i))) ? chars[i] : '-';
	}
	buf[9] = '\0';
}
