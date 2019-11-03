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

//定义3个信号量，用于避免同时使用共享内存，保证共享内存最多只有20个数据 
sem_t *mutex;
sem_t *empty;
sem_t *full;
//输入的参数
double l;
//定义只有20个数据的结构 
struct Buffer{
	long unsigned int buffer[20];
};
int j;
int fd;
struct Buffer *ptr;

//负指数分布随机数
double expntl(double x)
	{ 
		double z;
		do{
			z=((double)rand()/RAND_MAX);
		}while((z == 0)||(z == 1));
		return(-x*log(z));
}

//定义消费者的操作函数 
void cons(void *arg)
{	
	while(1){
		//保证只有在共享内存中有数据且没有别的线程在使用共享内存情况下进行操作 
		sem_wait(full);
		sem_wait(mutex);
		//获取信号量的值，确定数组的下标
		sem_getvalue(empty,&j);
		j=19 - j;
		//打印出进程、线程、消费数据的值
		printf("pid tid and data are %d %lu %lu \n", getpid(),pthread_self(), ptr->buffer[j]);
		//告知其他线程可以进行操作 
		sem_post(mutex);
		sem_post(empty);
		//延迟 
		sleep((int)(expntl(l))); 
	}
}

int main(int argc, char **argv)
{
	//定义消费者线程数组 
	pthread_t consumers[3];
	
	int i;
	//定义3个信号量为有名信号量用于两个进程间通信 
	mutex = sem_open("mutex",O_CREAT,0666,1);
	empty = sem_open("empty",O_CREAT,0666,20);
	full = sem_open("full",O_CREAT,0666,0);
	//打开有名共享内存，不存在则创建
	fd = shm_open("OS",O_CREAT | O_RDWR,0666);
	//设置共享内存大小 
	ftruncate(fd,sizeof(struct Buffer));
	//把一个Posix共享内存对象映射到调用进程的地址空间 
	ptr = mmap(NULL, sizeof(struct Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	//输入参数
	scanf("%lf",&l);
	
	//创建线程 
    for (i=0; i<3; i++) {
        pthread_create(&consumers[i], NULL, (void*)cons,NULL);
    }
     for (i=0; i<3; i++) {
        pthread_join(consumers[i], NULL);
    } 

}
