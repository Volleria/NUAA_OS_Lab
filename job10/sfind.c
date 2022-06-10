#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


void find_file(char *path, char *target)
{
    FILE *file = fopen(path, "r");

    char line[257];
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, target))
        {
            printf("%s: %s", path, line);
        }

    }
    fclose(file);
}

void find_dir(char *path, char *target)
{
    char newpath[257];
    DIR *dir = opendir(path);
    if(dir == NULL)
        return;
    struct dirent *entry;                      

    while (entry = readdir(dir))
    {
        sprintf(newpath, "%s/%s",path,entry->d_name);
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (entry->d_type == DT_DIR)
            find_dir(newpath,target);
        if (entry->d_type == DT_REG)
            find_file(newpath,target);          
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        puts("Usage: sfind file string");
        return 0;
    }

    char *path = argv[1];
    char *string = argv[2];

    struct stat info; 

    // 这个函数执行成功返回0，失败返回-1。
    // 取得的文件状态存放在 info 指针指向的struct stat结构体中
    stat(path, &info);

    // 其中， st_mode这个变量用来判断文件类型。
    // S_ISDIR(st_mode)  是否为目录
    if (S_ISDIR(info.st_mode))
        find_dir(path, string);
    else
        find_file(path, string);

    return 0;
}