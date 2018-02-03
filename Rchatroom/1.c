/*************************************************************************
	> File Name: 1.c
	> Author: hujialu
	> Mail: 
	> Created Time: 2017年08月28日 星期一 01时03分46秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
/*int main()
{
    char file_name[20];
    printf("请输入路径:\n");
    scanf("%s",file_name);
    char *q = file_name;
    int flag=0;
    file_name[strlen(file_name)+1]='\0';
    while(*q!='\0')
    {
        q++;
        if(*q=='/')
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1){
        char *p = file_name+strlen(file_name)-1;
        while(*p!='/')
        {
            p--;
        }
        p++;
        printf("%s\n",p);
    }
    else
    {
        char *p = file_name;
        printf("%s\n",p);
    }
}*/
int main()
{
    char *filename="/home/jialu/2/hh.txt";
    int fd;
    if((fd = (open(filename,O_RDONLY,S_IRUSR|S_IWUSR)))==-1)
    {
        printf("cuowu\n");       
    }
    if(lseek(fd,0,SEEK_END) == -1)
    {
        printf("sleek cuowu");
    }
    close(fd);
}
