#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

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

	// C 库函数 char *fgets(char *str, int n, FILE *stream) 
	// 从指定的流 stream 读取一行，并把它存储在 str 所指向的字符串内。
	// 当读取 (n-1) 个字符时，或者读取到换行符时，或者到达文件末尾时，它会停止，具体视情况而定。

	//1、strcat()
    //此函数原型为 char *strcat(char *dest, const char *src).
    //功能为连接两个字符串，把src连接到dest后面；返回dest地址

	//2、strcmp()
    //此函数的函数原型为 int strcmp(const char *str1, const char *str2)
    //功能为比较两个字符串。
    //当str1指向的字符串大于str2指向的字符串时，返回正数。
    //当str1指向的字符串等于str2指向的字符串时，返回0。
    //当str1指向的字符串小于str2指向的字符串时，返回负数。

	//3、strcpy()
    //此函数原型为 char *strcpy(char* dest, const char *src)
    //功能为拷贝字符串内容到目的串，把src所指向的内容拷贝到dest

	//函数原型为char *strstr(const char str1, const char *str2)
	//功能为查找字符串str2在str1中出现的位置，找到则返回位置，否则返回NULL 

	//parent: close(fd[0]) // 父进程关闭读端
    //parent: write(fd[1], &buf, sizeof(buf)); // 父进程往写端写入数据
    //child: close(fd[1]); // 子进程关闭写端
    //child: read(fd[0], &buf, sizeof(buf)); // 子进程从读端读取数据


    fgets(commandbuffer, 100 ,stdin);

	char *p;
	char *argv[10];
	int i = 0;

	printf("%ld",sizeof(argv));
	p = strtok(commandbuffer, " ");
	while(p != NULL) 
	{
		argv[i++] = p;
		p = strtok(NULL, " ");
	}
	argv[i] = NULL;
	int j = 0;
	while (argv[j])
	{
		printf("%s\n",argv[j]);
		j++;
	}
	


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
		// 字符串转换，将commadbuffer中去除“cd ”后的字符串存到tragetdir中 
		sscanf(commandbuffer,"cd %s",targetdir);
		printf("%s",targetdir);
		chdir(targetdir);		
    }      
	else
		mysys(commandbuffer);

    return 0;
}
