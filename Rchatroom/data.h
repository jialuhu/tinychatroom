/*************************************************************************
	> File Name: data.h
	> Author: 
	> Mail: 
	> Created Time: 四  8/10 15:13:57 2017
 ************************************************************************/

#ifndef _DATA_H
#define _DATA_H
#include<mysql.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<time.h>
/*struct connect
{
   int connect_id;
   char connect_userid[16];
   struct connect *next;
};*/
struct chat
{
    char user_name[20];//用户名称
    char user_passwd[16];//用户密码
    char user_id[20];//用户ID
    int command;//用户选择命令
    char to_id[20];//发送对象ID
    char status[3];//包括登录在线，离线消息，文件消息
    int back;
    int lens;
    char datas[225];
    char masg[200];
};
typedef struct chat stchat;
typedef struct chat *pstchat;

enum
{
    REGIST = 0,
    LOGIN = 1,
    HELP = 2,
    EXIT = 3,
};
enum
{
    ONLINE=1,
    N_ONLINE = 0,

};
enum{
    add_friend=1,
    delete_friend=2,
    look_online=3,
    look_outline=4,
    person_chat=5,
    all_chat=6,
    look_circle=7,
    add_circle=8,
    out_circle=9,
    check_imformation=10,
    friend_imformation=11,
    system_imformation=12,
    deal_aply=13,
    create_circle=14,
    close_circle=15,
    look_circle_history=16,
    send_files=17,
   // delete_friend=18,
};
/*int create_sock()
{
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
        my_eer("socket",__LINE__);
    }
    else
    {
        return sock_fd;
    }
}
void connect(int sock_fd,struct sockaddr_in *ser)
{
    if(connect(sock_fd, (struct sockaddr *)ser, sizeof(struct sockaddr)) < 0)
    {
        my_eer("connect",__LINE__);
        exit(1);
    }
}

void write(int sock_fd,struct sockaddr_in *temp)
{
    int n;
    n = write(sock_fd, temp, sizeof(stchat));
    if(n<0)
    {
        my_eer("write",__LINE__);
    }
}*/
//int create_sock();
//void connect(int sock_fd,struct sockaddr_in *ser);
//void write(int sock_fd,struct sockaddr_in *temp);
#endif
