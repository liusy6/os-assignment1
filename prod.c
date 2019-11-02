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
#include <assert.h>
#include <sys/shm.h>
#include <sys/sem.h>
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
int fd;
struct Buffer *ptr;

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
void prod(void *arg)
{	
	while(1){
		//�ڹ����ڴ������ݲ���20����û���߳����ù����ڴ������²�д��		
		sem_wait(empty);
		sem_wait(mutex);
		//��ȡ�ź�����ֵ��ȷ��������±� 
		sem_getvalue(full,&j);
		//�������һ��ֵ 
		ptr->buffer[j] = rand();
		//��ӡ�����̡��̡߳��������ݵ�ֵ 
		printf("pid tid and data are %d %lu %lu \n", getpid(),pthread_self(), ptr->buffer[j]);
		//�������߳�֪���ɶԹ����ڴ���в��� 
		sem_post(mutex);
		sem_post(full);
		//�����ӳٲ��� 
		sleep((int)(expntl(l)));
	}
}

int main(int argc, char **argv)
{
	//�����������߳����� 
	pthread_t producers[3];
	
	int i;
	//����3���ź���Ϊ�����ź��������������̼�ͨ�ţ������ʼֵ 
	mutex = sem_open("mutex",O_CREAT,0666,1);
	empty = sem_open("empty",O_CREAT,0666,20);
	full = sem_open("full",O_CREAT,0666,0);
	//�����������ڴ棬�������򴴽�
	fd = shm_open("OS",O_CREAT | O_RDWR,0666);
	//���ù����ڴ��С
	ftruncate(fd,sizeof(struct Buffer));
	//��һ��Posix�����ڴ����ӳ�䵽���ý��̵ĵ�ַ�ռ�
	ptr = mmap(NULL, sizeof(struct Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	//������� 
	scanf("%lf",&l);
		
	//�����߳� 
    for (i=0; i<3; i++) {
        pthread_create(&producers[i], NULL, (void*)prod,NULL);
    }
     for (i=0; i<3; i++) {
        pthread_join(producers[i], NULL);
    } 

}
