#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

//int dup(int oldfd);
//通过复制文件描述符oldfd，创建一个新的文件描述符newfd
//newfd和oldfd指向相同的文件

//int dup2(int oldfd, int newfd);
//通过复制文件描述符oldfd，创建一个新的文件描述符newfd
//newfd和oldfd指向相同的文件

// exit(100);
// 通过特殊的环境变量$?获得程序的退出码
// 程序调用exit(100)退出，退出码是100

void mysys(char const* command) 
{
	pid_t pid;
	pid = fork();
	if(pid == 0)
		execl("/bin/sh","sh","-c",command,NULL);
    wait(NULL);
}
int main()
{
    printf("> ");
    char commandbuffer[100];
    fgets(commandbuffer, 100 ,stdin);
    
	if( !strcmp(commandbuffer , "exit"))
		exit(2);
	else if(! strcmp(commandbuffer,"pwd"))
	{
		char * path = getcwd(NULL,0);
		printf("%s",path);
		free(path);
	}
       else if(commandbuffer[0]=='c'&& commandbuffer[1]=='d')
       {
		char targetdir[256];
		sscanf(commandbuffer,"cd %s",targetdir);
		printf("%s",targetdir);
		chdir(targetdir);		
       }
       
	else
		mysys(commandbuffer);

    return 0;
}
