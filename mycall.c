#include <linux/kernel.h> 
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#define _SYSCALL_MYSETNICE_ 436 //����ϵͳ���ú� 

int main()
{
	//����ϵͳ���� 
	syscall(_SYSCALL_MYSETNICE_); 
	return 0; 
}
