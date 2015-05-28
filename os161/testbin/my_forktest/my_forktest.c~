/*
 * forktest - test fork().
 *
 * This should work correctly when fork is implemented.
 *
 * It should also continue to work after subsequent assignments, most
 * notably after implementing the virtual memory system.
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>


int
main(int argc, char *argv[])
{
	
	//int return_write = write(1,"qwerty\n",4);	
	
	
	
	int pid;
	int exit_status_child;
	int returning_pid;
	
	write(1,"This is fork and exit test\n",30);
	pid = (int) fork();
	if(pid == 0)//in child
	{
		printf("my_forktest.c: I am a child and my pid is  %d\n",getpid());
		exit(15);
		
	}
		
	else//in parent
	{
		printf("my_forktest.c: I am a parent having pid %d and my child has pid %d\n",getpid(),pid);
		//waitpid(pid,&exit_status_child,0);
		//printf("Im in parent and exit code of my child is %d",exit_status_child);
		exit(2);
	}
	return 0;
}
