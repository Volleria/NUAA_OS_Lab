#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX 65536

int main(int argc , char * argv[])
{
	char buffer[MAX];
	int fd1 = open(argv[1], O_RDONLY);
	int count = read(fd1 , buffer, sizeof(buffer));
	close(fd1);

	int fd2 = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC);

	write(fd2, buffer,count);
	close(fd2);

	return 0;

}

