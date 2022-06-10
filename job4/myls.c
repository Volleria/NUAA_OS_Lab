#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	DIR *dp = opendir(argv[1]);
	struct dirent *sdp;
	
	if(argv[1] == NULL)
	{
		char * path = getcwd(NULL , 0);
		dp = opendir(path);
	}
	else if( dp  == NULL  )
	{
		perror("opendir error");
		exit(1);
	}

	while(sdp = readdir(dp) )
	{
		if( !strcmp(sdp-> d_name , ".")|| !strcmp(sdp->d_name ,"..") )
			continue;
		printf("%s\n", sdp->d_name);
	}

	printf("\n");
	closedir(dp);
	return 0;
}
