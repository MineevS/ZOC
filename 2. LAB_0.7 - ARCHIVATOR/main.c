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

void func_input_in_arch(int argc,char** argv,char* directory, DIR* dir,struct dirent* entry, struct stat file_st);
void func_write_in_arch(int argc,char** argv,int i, FILE* file_arch, struct stat file_st, size_t count_files);
void fmode(mode_t mode, char* buf);
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

char* strmemcp(char* str)
{
	char* mem = malloc(strlen(str)*sizeof(char));
	memset(mem, 0, (strlen(str) + 1)*sizeof(char));
	strncpy(mem, str, strlen(str));

	return mem;
}

size_t contains_symbol(char *symbols, char symbol){
  size_t pos = 0;
  if(symbols == NULL)
    return 0;
  while(*symbols != '\0'){
    if(*symbols++ == symbol)
      pos++;
  }
  return pos;
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
							FILE *file_arch;
							int fd = dirfd(dir);
							struct stat file_arch_st;

							if(strncmp(strrev(strmemcp(file_name)), strrev(strmemcp(".kmb")), 4) != 0) continue;
							if((file_arch = fopen(file_name, "rb")) != 0)
							{
								f = 1;
								fstatat(fd, file_name, &file_arch_st, 0);
								
								if(file_arch_st.st_size == 0)
								{
									printf("Файл пуст!\n");
								}
								else
								{
									printf("Файл не пуст!\n");
									
									for(size_t j = 0; j < argc; j++)
									{
										int fl = 1;
										
										if((stat(argv[j], &file_st))!= 0)
										{
											printf("Файл с именем %s уже существует в той директории, где находится и сам архив!\n", argv[j]);
										}

										size_t a = 0; long pos = 0;                          
										fseek(file_arch,pos,SEEK_SET);   
		 								// Читать счетчик
										fread((void*)&a, sizeof(size_t),1,file_arch);

										////////////////////////////////////////////
										size_t len_name_file = 0;
										char* buff = (char *)malloc(sizeof(char)*len_name_file);
										size_t b, c, d;
										size_t size_f_1 = 0;
										size_t size_f_2 = 0;
										
										size_t fb=0;
										mode_t fmode;
											
										for(size_t w = 0; w < a; w++)
										{
											
											fread(&len_name_file, sizeof(size_t),1,file_arch);
											memset(buff, 0, len_name_file + 1);
											fread(buff, sizeof(char), len_name_file, file_arch);//Получение имени файла	
											fread((void*)&b, sizeof(size_t), 1, file_arch);// Получение размера файла
											
											if(fl){
												if((strcmp(argv[j], buff)) == 0){fl = 0;}
												else{size_f_1 += b;}
											}
											
											if((strcmp(argv[j], buff)) == 0){fb = b;}
											size_f_2 += b;

											mode_t mt;//Получение режима файла.
											if((strcmp(argv[j], buff)) == 0)
											{
												fread(&fmode, sizeof(mode_t), 1, file_arch);// Получение режима файла
											}else{
												fread(&mt, sizeof(mode_t), 1, file_arch);
											}
											
											c = ftell(file_arch);// Текущее положение курсора
											fseek(file_arch, 0, SEEK_END);
											d = ftell(file_arch);// Размер всего файла
											fseek(file_arch, c, SEEK_SET);
										}

										if((((d - c) == size_f_2) && (!fl)))
										{
											printf("Создание файла: %s\n", argv[j]);
											
											fseek(file_arch, c + size_f_1, SEEK_SET);
											// TODO;
											int size_mas = fb;
											char * mas = (char*)malloc(size_mas*sizeof(char));
											memset(mas,0,sizeof(char)*size_mas + 1);
											fread(mas, sizeof(char), fb, file_arch);
											
											if(strstr(argv[0], argv[j]) != NULL) continue;// argv[0]="./gdb", argv[j]="gdb"
											
											FILE* file_f;
											if((file_f = fopen(argv[j], "w+b"))!= NULL){
												
												fwrite(mas,sizeof(char),fb, file_f);
												
												chmod(argv[j], fmode);
											}
											
											fclose(file_f);
										}
									}
								}

								fclose(file_arch);
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
							FILE *file_arch;
							int fd = dirfd(dir);
							struct stat file_arch_st;

							if(strncmp(strrev(strmemcp(file_name)), strrev(strmemcp(".kmb")), 4) != 0) continue;
							if((file_arch = fopen(file_name, "rb")) != 0)
							{
								f = 1;
								fstatat(fd, file_name, &file_arch_st, 0);
								
								if(file_arch_st.st_size == 0)
								{
									printf("Архив пуст!\n");
								}
								else
								{
									//printf("Файл не пуст!\n");
									
									size_t a = 0; long pos = 0;                          
									fseek(file_arch,pos,SEEK_SET);   
		 							// Читать счетчик
									fread((void*)&a, sizeof(size_t),1,file_arch);

									////////////////////////////////////////////
									size_t len_name_file = 0;
									char* buff = (char *)malloc(sizeof(char)*len_name_file);
									size_t b, c, d;
									size_t size_f_1 = 0;
									size_t size_f_2 = 0;
									
									size_t fb=0;
									printf("Обнажено %zu файлов в архиве: \n", a);
									
									for(size_t w = 1; w <= a; w++)
									{
										fread(&len_name_file, sizeof(size_t),1,file_arch);
										memset(buff, 0, len_name_file + 1);
										fread(buff, sizeof(char), len_name_file, file_arch);//Получение имени файла
										
										fread((void*)&b, sizeof(size_t), 1, file_arch);// Получение размера файла
			
										size_f_2 += b;

										mode_t mt;//Получение режима файла.
										fread(&mt, sizeof(mode_t), 1, file_arch);
										
										char buffm[10];
										memset(buffm, 0, 10);
										fmode(mt, buffm);
										
										printf("\033[1;31m%zu\033[0m.%s size:\033[1;36m%zu\033[0m  mode:\033[1;34m%s\033[0m\n", w, buff, b, buffm);
									}
								}

								fclose(file_arch);
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
			printf("В аргументах командной строки должен быть указан архив - файл с типом \".kmb\". явно!");
			
			printf("По умолчанию вызов команды с архивом и файлами автоматически добавляет файлы в архив!\n");
			break;
		case(4):
			printf("delete file(s)\n");

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
							FILE *file_arch;
							int fd = dirfd(dir);
							struct stat file_arch_st;

							if(strncmp(strrev(strmemcp(file_name)), strrev(strmemcp(".kmb")), 4) != 0) continue;

							if((file_arch = fopen(file_name, "rb")) != 0)
							{
								f = 1;
								fstatat(fd, file_name, &file_arch_st, 0);
								
								if(file_arch_st.st_size == 0)
								{
									//printf("?????");
								}
								else
								{
									//printf("??????");
									
									for(size_t j = 0; j < argc; j++)
									{
										int fl = 1;
										
										if((stat(argv[j], &file_st))!= 0)
										{
											//printf("??????s ?????????????? ????????\n", argv[j]);
										}

										size_t a = 0; long pos = 0;                          
										fseek(file_arch,pos,SEEK_SET);   
										fread((void*)&a, sizeof(size_t),1,file_arch);

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
											//len_name_file[w] =(size_t)realloc(len_name_file,sizeof(size_t));
											fread(&len_name_file[w], sizeof(size_t),1,file_arch);
											
											buff[w] =(char*)malloc(len_name_file[w]*sizeof(char));
											memset(buff[w], 0, len_name_file[w] + 1);
											fread(buff[w], sizeof(char), len_name_file[w], file_arch);//????????
											//fb[w] = (size_t) realloc(fb, sizeof(size_t));
											fread(&fb[w], sizeof(size_t), 1, file_arch);// ?????? ???										
											//fmode[w] = (mode_t) realloc(fmode, sizeof(mode_t));
											fread(&fmode[w], sizeof(mode_t), 1, file_arch);// ?????????										
											c = ftell(file_arch);// ?????? ???
											fseek(file_arch, 0, SEEK_END);
											d = ftell(file_arch);// Р???? ???
											fseek(file_arch, c, SEEK_SET);
										}

										if(a == 0)continue;

										char* mas;
										mas = (char*)malloc(sizeof(char)*(d - c));
										memset(mas, 0, (d - c) + 1);
										fread(mas, sizeof(char), (d - c), file_arch);
										int flg = 0;
										fclose(file_arch);

										if((file_arch = fopen(file_name, "wb")) != 0)
										{
											fwrite(&a, sizeof(size_t),1, file_arch);
											for(size_t w = 0; w < a; w++)
											{
												if(args_strcmp(buff[w], argv, argc) != 0)
												{

													fwrite(&len_name_file[w], sizeof(size_t),1,file_arch);
													fwrite(buff[w], sizeof(char),len_name_file[w],file_arch);
													fwrite(&fb[w], sizeof(size_t),1,file_arch);
													fwrite(&fmode[w], sizeof(mode_t),1, file_arch);

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
													size_t c_ = ftell(file_arch);
													fseek(file_arch,0, SEEK_SET);
													fwrite(&b_s, sizeof(size_t), 1, file_arch);
													fflush(file_arch);
													fseek(file_arch,c_, SEEK_SET);
												}
											}
											
											fwrite(mas, sizeof(char), size_f_1, file_arch);
											fwrite(mas+(size_f_1+fb[flg])*sizeof(char), sizeof(char), size_f_2, file_arch);
											fflush(file_arch);
										}
									}
								}

								fclose(file_arch);
							}
						}
					}
					
					closedir(dir);
				}	
			}

			break;

			
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
		int FLAG_ARCH = 0;
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
					FILE *file_arch;
					int fd = dirfd(dir);
					struct stat file_arch_st;
					int count_files = 0;

					if(strncmp(strrev(strmemcp(file_name)), strrev(strmemcp(".kmb")), 4) != 0) continue;

					if((file_arch = fopen(file_name, "r+b")) != 0)
					{
						FLAG_ARCH = 1;
						fstatat(fd, file_name, &file_arch_st, 0);

						if(file_arch_st.st_size == 0)
						{
							//printf("Файл пуст!\n");
							func_write_in_arch(argc, argv, i, file_arch, file_st, 0);
						}
						else
						{
							//printf("Файл не пуст!\n");
							//Обновить счетчик в двоичном файле
							size_t a = 0; long pos = 0;                          
							fseek(file_arch,pos,SEEK_SET);   
		 					// Читать счетчик
							fread((void*)&a, sizeof(size_t),1,file_arch);

							func_write_in_arch(argc, argv, i, file_arch, file_st, a);
						}
					}
					
					fclose(file_arch);
				}
			}
			
			
			if(!FLAG_ARCH){// Если архив не обнаружен!	

				//if(strncmp(strrev(strmemcp(argv[i])),strrev(strmemcp(".kmb")), 4) != 0) continue;

				if(strncmp(strrev(strmemcp(argv[i])),strrev(strmemcp(".kmb")), 4) != 0){
					num +=1;
				}
				
				printf("Архив с именем \033[35m%s\033[0m не обнаружен в данной директории\n", argv[i]);
				printf("Создать архив с именем \033[32m%s\033[0m: ", argv[i]);

				switch(getchar())
				{
				case('Y'):
					printf("Y \n");
					break;
				case('y'):
					printf("y \n");

					FILE* file_arch;

					if((file_arch = fopen(argv[i], "wb+")) != 0)// Работа с архиватором;
					{
						func_write_in_arch(argc, argv, i, file_arch, file_st, 0);
					}
					
					break;
				default:
					break;
				}
			}
		}
		
		if(num == argc){
			printf("В командной строке в качестве аргумента не обнаружен файл с типом \".kmb\".\n");
			printf("В аргументах командной строки должен быть указан архив - файл с типом \".kmb\". явно!");
			printf("Работа программы прекращена!");
		}
	}
}

void func_write_in_arch(int argc,char** argv,int i, FILE* file_arch, struct stat file_st, size_t count_files)
{
	//size_t count_files = 0;
	for(size_t j = 1; j < argc; j++)// Цикл аргументных имен файлов
	{
		// TODO;

		if(j == i) continue;//  т. к. argv[i] - имя arch_file

		FILE* file;
		
		if(((file = fopen(argv[j], "rb")) != 0) && (count_files == 0))
		{
			stat(argv[j], &file_st);
			printf("Успешно был открыт файл: \033[1;31m%s \033[0m\n", argv[j]);
			char* buff_file = (char*)malloc(file_st.st_size * sizeof(char));
			fread(buff_file, sizeof(char), file_st.st_size, file);
			count_files++;
			fwrite(&count_files, sizeof(size_t), 1, file_arch);
			size_t s = strlen(argv[j]);
			fwrite(&s, sizeof(size_t), 1, file_arch);
			fwrite(argv[j], sizeof(char), strlen(argv[j]), file_arch);
			fwrite(&file_st.st_size, sizeof(file_st.st_size), 1, file_arch);
			fwrite(&file_st.st_mode, sizeof(file_st.st_mode), 1, file_arch);
			fwrite(buff_file,sizeof(char),file_st.st_size, file_arch);
			fflush(file_arch);

			fclose(file);
		}
		else if(((file = fopen(argv[j], "rb")) != 0)&&(count_files > 0))
		{
			stat(argv[j], &file_st);

			printf("Успешно был открыт файл: \033[1;31m%s \033[0m\n", argv[j]);
			char* buff_file = (char*)malloc(file_st.st_size * sizeof(char));
			fread(buff_file, sizeof(char), file_st.st_size, file);
			count_files++;

			//Обновить счетчик в двоичном файле
			size_t a = 0; long pos = 0;                          
			fseek(file_arch,pos,SEEK_SET);   
		 	// Читать счетчик
			fread((void*)&a, sizeof(size_t),1,file_arch);
			a++; // Увеличить в памяти
			fseek(file_arch,pos,SEEK_SET);   // Записать обратно по тому же адресу
			fwrite((void*)&a, sizeof(size_t),1,file_arch);

			////////////////////////////////////////////
						
			size_t i_buff = ftell(file_arch);

			size_t count = 1;
			char* buff = (char *)malloc(sizeof(char)*count);
			memset(buff, 0, sizeof(char)*count + 1);

			char** files_mas = (char**)calloc(0, sizeof(char*));
			size_t b, c, d;
			size_t size_f = 0;
			char buff_new_filename[256] = {0};
			for(size_t w = 1; w < a; w++)
			{
							
				size_t len_name_file = 0;
				fread(&len_name_file, sizeof(size_t),1,file_arch);
				fread(buff, sizeof(char), len_name_file, file_arch);

				if(strcmp(buff, argv[j]) == 0)
				{
					printf("В архиве обнаружен файл с похожим именем %s\n", argv[j]);
					printf("Введите новое имя файла для возможности сохранения файла в архив!: ");
					scanf("%s", buff_new_filename);
					while(strlen(buff_new_filename) > 256)
					{
						printf("Введите новое имя файла для возможности сохранения файла в архив c неболее 255 символами!");
						scanf("%s", buff_new_filename);
					}

					printf("Данные учтены!");
				}

				fread((void*)&b, sizeof(size_t), 1, file_arch);
				size_f += b;

				mode_t mt;//Получение режима файла.
				fread(&mt, sizeof(mode_t), 1, file_arch);
								
				c = ftell(file_arch);// Текущее положение курсора

				fseek(file_arch, 0, SEEK_END);

				d = ftell(file_arch);// Размер всего файла
					
				fseek(file_arch, c, SEEK_SET);
			}

			if((d - c) == size_f)
			{
				fseek(file_arch, c, SEEK_SET);
				int cs = ftell(file_arch);
				int size_mas = d - cs;
				char * mas = (char*)malloc(size_mas*sizeof(char));
				fread(mas, sizeof(char), size_mas, file_arch);
				fseek(file_arch,cs, SEEK_SET);
				size_t st = 0;

				if(strlen(buff_new_filename) >0)
				{

					st = strlen(buff_new_filename);
					fwrite(&st,sizeof(size_t),1, file_arch);
					fwrite(buff_new_filename, sizeof(char), st, file_arch);

				}
				else
				{
					st = strlen(argv[j]);
					fwrite(&st,sizeof(size_t),1, file_arch);
					fwrite(argv[j], sizeof(char), st, file_arch);
				}

				fwrite(&file_st.st_size, sizeof(file_st.st_size), 1, file_arch);					
				fwrite(&file_st.st_mode, sizeof(mode_t),1, file_arch);
				fwrite(mas, sizeof(char), size_mas, file_arch);

				fwrite(buff_file,sizeof(char),file_st.st_size, file_arch);
				fflush(file_arch);
			}

			fflush(file_arch);

			fclose(file);
		}
	}
}

void file_seek(FILE* file, int num_str, int stat)
{
	fflush(file);
	fseek(file, 0, SEEK_SET);

	if(stat == 1)
	{
		int size_n = 0;
		
		for(size_t i = 0; i < num_str; i++)
		{
			if(feof(file))
			{ 
				break;
			}

			char c;
			char* buffer;
			size_t bufsize = 0;
			size_t char_size = 0;

			char_size = getdelim(&buffer, &bufsize,'\n' , file);

			if(strlen(buffer) > 0) size_n++;
			
			if(num_str == size_n)
			{
				fseek(file, ftell(file) - 1*sizeof(char), SEEK_SET);
			}

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

int parse_to_int(nlink_t *source)
{
  int buffer_size = sizeof(*source);
  char buffer[buffer_size];
  snprintf(&buffer[0], buffer_size, "%lu", (unsigned long)source);
  int val = atoi(buffer);
  return val;
}

