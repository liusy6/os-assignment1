#include <linux/kernel.h> 
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#define _SYSCALL_MYSETNICE_ 436 //定义系统调用号 

int main()
{
	//进行系统调用 
	syscall(_SYSCALL_MYSETNICE_); 
	return 0; 
}
