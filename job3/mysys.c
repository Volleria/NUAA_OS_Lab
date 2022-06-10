#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

void mysys(char const* command) 
{
	pid_t pid;
	pid = fork();
	if(pid == 0) // 如果是子进程
		execl("/bin/sh","sh","-c",command,NULL);
    wait(NULL);
}
int main()
{
    
    printf("-----------------------------\n");
    mysys("echo HELLO WORLD");
    printf("-----------------------------\n");
    mysys("ls /");	
    printf("------------------------------\n");
    return 0;
}
