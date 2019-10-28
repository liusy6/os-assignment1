#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

//线程互斥量
pthread_mutex_t forks[5];
pthread_cond_t var[5];

int philosophers[5] = {0, 1, 2, 3, 4};

//哲学家状态 
enum {THINKING, HUNGRY,EATING} state[5];

//声明操作函数,取叉函数,放叉函数,测试函数 
void* philospher(void *arg);
void pick_forks(int philosopher_number);
void return_forks(int philosopher_number);
int test(int); 

//延迟函数
void delay () {
	float i=0;
	while (i<1){
    	i = rand() % 3;
    }
    sleep(i);
}

//测试函数 
int test(int i){
	if(state[i] == HUNGRY && state[(i+4) % 5] != EATING && state[(i+1) % 5] != EATING)
	{
		return 1;
	}
	return 0;
}

void pick_forks(int philosopher_number){
	state[philosopher_number]=HUNGRY;
	pthread_mutex_lock(&forks[philosopher_number]);  
	while (test(philosopher_number) == 0){
		printf("哲学家%d正在等待\n",philosopher_number);
		pthread_cond_wait(&var[philosopher_number] , &forks[philosopher_number]); //等待条件变量才上锁，避免死锁 
	}
	state[philosopher_number]=EATING;
}

void return_forks(int philosopher_number){
	state[philosopher_number]=THINKING;
	pthread_cond_signal(&var[philosopher_number]);
	//对进食完的哲学家的左右哲学家条件变量进行改变 
	pthread_cond_signal(&var[(philosopher_number+4) % 5]);
	pthread_cond_signal(&var[(philosopher_number+1) % 5]);
	pthread_mutex_unlock(&forks[philosopher_number]);
}

//定义函数 
void* philospher(void *arg){
	int i = *(int *)arg;
	while(1){
		printf("哲学家%d正在思考问题\n", i);
		delay();
		printf("哲学家%d饿了\n", i);
		pick_forks(i);
		printf("哲学家%d正在进食\n", i);
		delay();
		printf("哲学家%d吃完了\n",i);
		return_forks(i);
	}
}


int main (int argc, char **argv) {
    pthread_t PHD[5];//5个线程 
    int i; 
    //定义哲学家的初始状态 
    for (i=0; i<5; i++) {
        state[i]=THINKING;
    }
    //初始化互斥量 
    for (i=0; i<5; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
    //初始化条件变量 
    for (i=0; i<5; i++) {
        pthread_cond_init(&var[i],NULL);
    }
    //创建线程 
    for (i=0; i<5; i++) {
        pthread_create(&PHD[i], NULL, (void*)philospher, &philosophers[i]);
    }
     for (i=0; i<5; i++) {
        pthread_join(PHD[i], NULL);
    } 
}
    
