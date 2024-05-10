/***********************************************************************************
# File Name:    116-文件复制.c
# Author:       dengiswen
# Mail:         dengiswen@gmail.com
# Created Time: 2024年05月08日 星期三 17时49分13秒
 **********************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#define NUMS 2
void* readDemo(void*);
void* writeDemo(void*);
sem_t sem[2];
char data[100];//保存一行的内容
int main(){
	//初始化sem
	sem_init(&sem[0],0,0);
	sem_init(&sem[1],0,0);
	//创建线程
	pthread_t pids[2];
	pthread_create(&pids[0],NULL,readDemo,NULL);
	pthread_create(&pids[1],NULL,writeDemo,NULL);
	//等待线程
	pthread_join(pids[0],NULL);
	pthread_join(pids[1],NULL);
	puts("复制完毕");
}
void * readDemo(void *args){
	FILE *fp =fopen("1.txt","r");
	if(fp==NULL){
		perror("文件打开失败");
		exit(0);
	}
	while(fgets(data,sizeof(data),fp)!=NULL){
		printf("读线程执行.....\n");
		sem_post(&sem[1]);//写的信号量+1
		puts("post之后 代码");
		sem_wait(&sem[0]);//等待读信号量
		memset(data,0,sizeof(data));
	}
	//防止写线程死锁的情况 
	sem_post(&sem[1]);
	return NULL;
}
void * writeDemo(void *args){
	FILE *fp =fopen("2.txt","a");
	if(fp==NULL){
		perror("文件打开失败");
		exit(0);
	}
	sem_wait(&sem[1]);
	while(strcmp(data,"")!=0){
		puts("写线程执行中");
		fputs(data,fp);
		sem_post(&sem[0]);
		sem_wait(&sem[1]);
	}
	return NULL;
}


