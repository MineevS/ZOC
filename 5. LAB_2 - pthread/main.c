#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

//typedef unsigned char uchar;
#define SHARED_MAS_SIZE 32
//128

typedef struct DATA
{
	int id;
	//int data[SHARED_MAS_SIZE];
	int* data;
} DATA;

void* thread_func(void* arg)
{
	//printf("I'm ", pthread_self());
	
	//int* data =(int*)arg;
	//printf("Data_from Main: %d - %d", data[i], i)
	//show_mas(data, SHARED_MAS_SIZE, "Tread");
	DATA* data =(DATA*)arg;
	//printf("Data_from Main: %d - %d", data[i], i)
	show_mas(data->data, SHARED_MAS_SIZE, data->id);
	
	printf("Pointer: %p \n", data->data);
	
	usleep(10000);
	
	pthread_exit(NULL);
}
/*
void show_mas(int* mas, int size, const char* from)
{
	for(int i = 0; i < SHARED_MAS_SIZE; i++){
		printf("%d -%d", data[i], i)
	}
}*/

void show_mas(int* mas, int size, int id)
{
	if(mas == NULL)
	{
		printf("ERROR \n");
		return -1;
	}

	for(int i = 0; i < SHARED_MAS_SIZE; i++){
		printf("%d -%d - id", data[i], i,id)
	}
}

int main(int argc, char** argv)
{
	pthread_t thread_1_id;
	pthread_t thread_2_id;
	
	int* data = (int*)malloc(SHARED_MAS_SIZE);
	printf("Pointer: %p \n", data->data);
	DATA data_struct_1;
	DATA data_struct_2;
	
	for(int i = 0; i < SHARED_MAS_SIZE; i++){data[i] = i;};
	show_mas(data, SHARED_MAS_SIZE, "Main");
	
	data_struct_1.id = 1;
	data_struct_2.id = 2;
	
	data_struct_1.data = data;
	data_struct_2.data = data;
	
	printf("Pointer: %p \n", data_struct_1.data);
	printf("Pointer: %p \n", data_struct_2.data);
	
	///////////
	int status_1 = pthread_create(&thread_1_id, NULL, thread_func, &data_struct_1);//&(void*)//data//&data
	//int status_2 = pthread_create(&thread_2_id, NULL, thread_func, &data_struct_2);
	int status_2 = 0;
	
	if((status_1 != 0) || (status_2 != 0))
	{
		printf(": %d - %d", status_1, status_2);
		return -1;
	}
	
	pthread_join(thread_1_id, NULL);
	pthread_join(thread_2_id, NULL);
	
	sleep(1);
	//printf("hello world!");

	free(data);
	
	return 0;
}