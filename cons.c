#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <malloc.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/ipc.h>
#include<assert.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include <math.h>

//����3���ź��������ڱ���ͬʱʹ�ù����ڴ棬��֤�����ڴ����ֻ��20������ 
sem_t *mutex;
sem_t *empty;
sem_t *full;
//����Ĳ���
double l;
//����ֻ��20���������Ľṹ 
struct Buffer{
	long unsigned int buffer[20];
};
int j;
int shmid;
void *ptr;

//��ָ���ֲ������
double expntl(double x)
	{ 
		double z;
		do{
			z=((double)rand()/RAND_MAX);
		}while((z == 0)||(z == 1));
		return(-x*log(z));
}

//���������ߵĲ������� 
void cons(void *arg)
{	
	while(1){
		//��ֻ֤���ڹ����ڴ�����������������û����ʹ�ù����ڴ� 
		sem_wait(full);
		sem_wait(mutex);
		//��ȡ�ź�����ֵ��ȷ��������±�
		sem_getvalue(empty,&j);
		j=19 - j;
		
		struct Buffer* message = (struct Buffer*)(ptr);
		//��ӡ�����̡��̡߳��������ݵ�ֵ
		printf("pid tid and data are %d %lu %lu \n", getpid(),pthread_self(), message->buffer[j]);
		//��֪�����߿��Խ��в��� 
		sem_post(mutex);
		sem_post(empty);
		//�ӳ� 
		sleep((int)(expntl(l))); 
	}
}

int main(int argc, char **argv)
{
	//�����������߳����� 
	pthread_t consumers[3];
	
	int i;
	//����3���ź���Ϊ�����ź��������������̼�ͨ�� 
	mutex = sem_open("mutex",0,0666,1);
	empty = sem_open("empty",0,0666,20);
	full = sem_open("full",0,0666,0);
	//���������ڴ棬�������򴴽�
	shmid = shmget(1234, sizeof(long unsigned int), IPC_CREAT|0666);
	//�����Ըù����ڴ�ķ���,���ѹ����ڴ����ӵ���ǰ���̵ĵ�ַ�ռ� 
	ptr = shmat(shmid, NULL, 0);
	//�������
	scanf("%lf",&l);
	
	//�����߳� 
    for (i=0; i<3; i++) {
        pthread_create(&consumers[i], NULL, (void*)cons,NULL);
    }
     for (i=0; i<3; i++) {
        pthread_join(consumers[i], NULL);
    } 

}
