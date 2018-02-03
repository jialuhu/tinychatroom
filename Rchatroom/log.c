/*************************************************************************
	> File Name: log.c
	> Author: hujialu
	> Mail: 
	> Created Time: 2017年08月16日 星期三 14时41分15秒
 ************************************************************************/

#include<stdio.h>
#include"data.h"
#include"serve.h"

#define HOST "localhost"
#define USER "root"
#define DB_NAME "chatroom"


MYSQL *my_sql_init(MYSQL *mysql)//数据库的初始化
{
    printf("mysql\n");
    mysql = mysql_init(NULL);
    if(!mysql)
    {
        printf("wucuo\n");
    }
    printf("successe\n");
    return mysql;
}

MYSQL *mysql_connect(MYSQL *mysql)//数据库的连接
{
    printf("mysql connetc\n");
    if(!mysql_real_connect(mysql, HOST, USER, NULL, DB_NAME, 0, NULL, 0))
    {
        printf("Failed to connect with mysql:%s\n",mysql_error(mysql));
    }
    printf("connect is successful!\n");
    return mysql;
}


int login_find(MYSQL *mysql,struct chat *temp)
{
/*update chat_user set s_online='1' where id='bsc90526';*/
/*select *from chat_user where passwd='4' and name='dfs';*/
    char query[200]={0};
    char pasd[16]={0};
    char id[16]={0};
    char name[20]={0};
    MYSQL_RES *res;
    MYSQL_ROW *row;
    char *command="select *from chat_user where";
    sprintf(query,"%s passwd='%s' and id='%s';",command,temp->user_passwd,temp->user_id);
    printf("query:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to query:%s\n",mysql_error(mysql));

    }
    res = mysql_store_result(mysql);
    printf("\n");
    while((row=mysql_fetch_row(res)))
    {
        for(t=0;t<mysql_num_fields(res);t++)
        {
            if(t==0)
            {
                strcpy(name,(char *)row[t]);
                name[strlen((char *)row[t])+1]='\0';
                printf("name:%s\n",name);
                continue;
            }
            if(t==1)
            {
                strcpy(id,(char *)row[t]);
                printf("id:%s\n",id);
                continue;
            }
            if(t==2)
            {
                strcpy(pasd,(char *)row[t]);
                printf("pasd:%s\n",pasd);
                continue;
            }
            /*if(t==3)//已经登陆
            {
                printf("status:%s\n",(char *)row[t]);
                return 0;
            }*/
        }
        printf("\n");
    }
    if((strcmp(temp->user_id,id))!=0)
    {
        printf("tid:%s\tid:%s\n",temp->user_id,id);
        return -1;//账号错误
       }
    if((strcmp(temp->user_passwd,pasd))!=0)
    {
        return -2;//密码不匹配
    }
    else
    {
        strcpy(temp->user_name,name);
        
        printf("t_name:%s",temp->user_name);
        return 1;//登陆成功
    }
    mysql_free_result(res);
}




MYSQL *create_table(MYSQL *mysql,char *table_name)//建立用户表表头
{
    printf("jinru\n");
    char *head="CREATE TABLE IF NOT EXISTS ";
    char query[400]={0};
    char *lie="(name char(50) NULL, id char(16) NOT NULL, S_online char(200) NOT NULL, type char(3))ENGINE=InnoDB;";
    printf("hed:%s\n",head);
    MYSQL *res;
    MYSQL *row;

    memset(query,0,200);
    sprintf(query,"%s%s%s",head,table_name,lie);
    query[strlen(query)+1]='\0';
    printf("query2:%s\n",query);
    int t = mysql_real_query(mysql, query, strlen(query));
    if(t)
    {
        printf("Falied to query:%s\n",mysql_error(mysql));
        return 0;
    }
    printf("successful of create the user_table\n");
    return mysql;
}


MYSQL *insert_data(MYSQL *mysql,struct chat *temp)//向用户表中插入数据
{
    char *s1="INSERT INTO chat_user(name, id, passwd, s_online)VALUES(";
    char query[200];
    char *end=");";
    MYSQL *res;
    MYSQL *row;

    memset(query, 0, 200);

    sprintf(query, "%s'%s', '%s', '%s', '0'%s",s1,temp->user_name,temp->user_id,temp->user_passwd,end);
    printf("query:%s\n",query);
    int t = mysql_real_query(mysql, query, strlen(query));
    if(t)
    {
        printf("Falied to query:%s\n",mysql_error(mysql));
        return 0;
    }
    printf("successful\n");
    return mysql;
}


