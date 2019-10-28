#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

//�̻߳�����
pthread_mutex_t forks[5];
pthread_cond_t var[5];

int philosophers[5] = {0, 1, 2, 3, 4};

//��ѧ��״̬ 
enum {THINKING, HUNGRY,EATING} state[5];

//������������,ȡ�溯��,�Ų溯��,���Ժ��� 
void* philospher(void *arg);
void pick_forks(int philosopher_number);
void return_forks(int philosopher_number);
int test(int); 

//�ӳٺ���
void delay () {
	float i=0;
	while (i<1){
    	i = rand() % 3;
    }
    sleep(i);
}

//���Ժ��� 
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
		printf("��ѧ��%d���ڵȴ�\n",philosopher_number);
		pthread_cond_wait(&var[philosopher_number] , &forks[philosopher_number]); //�ȴ������������������������� 
	}
	state[philosopher_number]=EATING;
}

void return_forks(int philosopher_number){
	state[philosopher_number]=THINKING;
	pthread_cond_signal(&var[philosopher_number]);
	//�Խ�ʳ�����ѧ�ҵ�������ѧ�������������иı� 
	pthread_cond_signal(&var[(philosopher_number+4) % 5]);
	pthread_cond_signal(&var[(philosopher_number+1) % 5]);
	pthread_mutex_unlock(&forks[philosopher_number]);
}

//���庯�� 
void* philospher(void *arg){
	int i = *(int *)arg;
	while(1){
		printf("��ѧ��%d����˼������\n", i);
		delay();
		printf("��ѧ��%d����\n", i);
		pick_forks(i);
		printf("��ѧ��%d���ڽ�ʳ\n", i);
		delay();
		printf("��ѧ��%d������\n",i);
		return_forks(i);
	}
}


int main (int argc, char **argv) {
    pthread_t PHD[5];//5���߳� 
    int i; 
    //������ѧ�ҵĳ�ʼ״̬ 
    for (i=0; i<5; i++) {
        state[i]=THINKING;
    }
    //��ʼ�������� 
    for (i=0; i<5; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
    //��ʼ���������� 
    for (i=0; i<5; i++) {
        pthread_cond_init(&var[i],NULL);
    }
    //�����߳� 
    for (i=0; i<5; i++) {
        pthread_create(&PHD[i], NULL, (void*)philospher, &philosophers[i]);
    }
     for (i=0; i<5; i++) {
        pthread_join(PHD[i], NULL);
    } 
}
    
