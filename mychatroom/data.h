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
#include<unistd.h>
#include<errno.h>
struct chat
{
    char user_name[20];//用户名称
    char user_passwd[16];//用户密码
    char user_id[20];//用户ID
    int command;//用户选择命令
    char to_id[20];//发送对象ID
    char status[3];//包括登录在线，离线消息，文件消息
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
#endif
