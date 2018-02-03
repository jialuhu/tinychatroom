/*************************************************************************
	> File Name: ceshi2.c
	> Author: 
	> Mail: 
	> Created Time: 三  8/ 9 21:12:12 2017
 ************************************************************************/
#include"data.h"
#include<pthread.h>
#define LISTENQ 12
int o=0;
void *dosome(void *arg)
{
    int conn_fd=(int)arg;
    stchat temp;
    char w_buf[32]="hello word!";
    int n = read(conn_fd,&temp,sizeof(stchat));
    write(conn_fd,w_buf,32);
    memset(w_buf,0,32);
    if(n==0)
    {
        printf("用户已经下线\n");
    }
    else{
        int choise=temp.command;
        switch(choise)
        {
            case 1:
            {
       // char *p=(char *)malloc(sizeof(stchat));
       // p = (char *)&temp;
                printf("cmd=%d\n",temp.command);
                printf("id=%s\n",temp.user_id);
                printf("name=%s\n",temp.user_name);
                printf("conn_fd:%d\n",conn_fd);
                printf("进入命令1\n");
                break;
            }
            case 2:
            {
                printf("two\n");
                break;
            }
        }
    }
    return 0;

}
int send_m(int tofd,int myfd)
{
    char re_buf[32];
    int n;
    memset(re_buf,0,32);
    n = read(myfd,re_buf,32);
    printf("re_buf:%s\n",re_buf);
    n = write(tofd,re_buf,32);
    printf("n=%d\n",n);
    return 1;
}

int main(int argc,char **argv)
{
    int sock_fd,conn_fd;//服务器套接字，客户端套接字
    struct sockaddr_in cli_add,ser_add;//服务器，客户端的地址结构
    char re_buf[32];
    pid_t pid;//创建线程
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    memset(&ser_add,0,sizeof(struct sockaddr));
    ser_add.sin_family=AF_INET;
    ser_add.sin_port=4507;
    ser_add.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock_fd,(struct sockaddr *)&ser_add,sizeof(struct sockaddr_in));//绑定服务器套接字
    listen(sock_fd,LISTENQ);//服务器 监听
    socklen_t len = sizeof(struct sockaddr_in);//客户端的地址结构的长度
    while(1)
    {
        conn_fd = accept(sock_fd,(struct sockaddr *)&cli_add,&len);//接收客户端请求
        pthread_t thid;
        if(conn_fd<0)
        {
            printf("accept failed\n");
        }
        else
        {
            printf("accept success\n");
            pthread_create(&thid,NULL,(void *)dosome,(void *)conn_fd);
        }

    }
}
