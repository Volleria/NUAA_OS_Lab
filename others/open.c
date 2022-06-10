#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd;
    fd = open("log", O_RDONLY);

    char buf[7];
    int a[10];
    int count = read(fd, buf, sizeof(buf));
    printf("count:%d\n",count);
    buf[count] = 0;
    printf("%s\n",buf);
    read(fd, buf, sizeof(buf));
    printf("----\n");
    printf("%s\n",buf);


    close(fd);
    return 0;
}
