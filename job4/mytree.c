#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>


void tree(char *path,int n)
{
	DIR *dir = opendir(path);
	
	char tab[n];
	memset(tab,' ', n);

	struct dirent *read;
	while(  read = readdir(dir) )
	{
		if(strcmp(read->d_name, ".") != 0 && strcmp(read->d_name, "..") != 0)
		{
		
			write(1, tab, n); 
			int length = strlen(read->d_name);
			write(1,read->d_name,length);
			write(1,"\n",1);
			
			if(read->d_type == DT_DIR)
			{
				sprintf(path , "%s/%s",path,read->d_name);
				tree(path, n+4);
			}
		}		
	}
}

int main(int argc, char *argv[])
{

	char path[256];
	if(argc == 1)
	{		
		write(1,".",1);
		getcwd(path,256);
	}
	else
	{
		strcpy(path, argv[1]);
        int length = strlen(path);      
	    write(1,path, length);
	}
	write(1,"\n",1);
	tree(path,4);
	return 0;
}
