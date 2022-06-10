#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
int main(int argc, char * argv[])
{
	// int open(const char *pathname, int flags, mode_t mode 0666);
	// pathname:要打开的文件名（含路径，缺省为当前路径）
    // flags：以什么的方式打开，O_RDONLY（只读打开），O_WRONLY（只写打开），O_RDWR（可读可写打开）
	// O_APPEND 以追加模式打开 O_TRUNC 文件长度截断为0
    // mode:打开方式的数字表现形式，例如，0600是代表可读可写的意思，切记mode一定是在flags中
    // 使用了O_CREAT标志才能用，mode记录待创建的文件访问权限

	// ssize_t write(int fd, const void *buf, size_t count);
	// write函数返回的是写入到文件里的字节数
    // 意思是从字符串buf中读取count个字节写到fd文件中。

	// ssize_t read(int fd, void *buf, size_t count);
	// 成功时，返回读取的字节数（零表示结束）出现错误时，返回-1

	// off_t lseek(int fd, off_t offset, int whence);//打开一个文件的下一次读写的开始位置
	// __off_t t = lseek(fd, 0, SEEK_END); 文件大小
	//  lseek()函数会重新定位被打开文件的位移量，根据参数offset以及whence的组合来决定：
    // SEEK_SET:从文件头部开始偏移offset个字节。
    // SEEK_CUR：从文件当前读写的指针位置开始，增加offset个字节的偏移量。
    // SEEK_END：文件偏移量设置为文件的大小加上偏移量字节。

	struct stat st;
	char *buffer;
	int fd = open(argv[1],O_RDONLY);	
	stat(argv[1],&st);
	int fsize = st.st_size;
	buffer = (char*)malloc( (1 + fsize) * sizeof(char));
	read (fd , buffer ,fsize);
	write(1 , buffer ,fsize);
	close(fd);
	return 0;
}

