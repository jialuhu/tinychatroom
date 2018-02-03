/*************************************************************************
	> File Name: sql.c
	> Author: hujialu
	> Mail: 
	> Created Time: 2017年08月17日 星期四 09时26分56秒
 ************************************************************************/


#include"data.h"
#define HOST "localhost"
#define USER "root"
#define PASSWD " "
#define DB_NAME "chatroom"
MYSQL find_online(MYSQL *mysql,struct chat *temp);//查看好友是否在线
int deletefriends(MYSQL *mysql,struct chat *temp);

//MYSQL *mysql;
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
 if(!mysql_set_character_set(mysql,"utf8"))
    {
        printf("new client charact set :%s\n",mysql_character_set_name(mysql));
    }
    printf("connect is successful!\n");
    return mysql;
}

MYSQL find_reg(MYSQL *mysql,struct chat *temp) //判断是否有该好友
{
    printf("temp:%s\n",temp->to_id);
    MYSQL_RES *res;
    MYSQL_ROW *row;
    char query[200]={0};
    char id[16]={0};
    strcpy(id,temp->to_id);
    sprintf(query,"select *from chatuser where id='%s';",id);
    printf("check_online query:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to query:%s\n",mysql_error(mysql));
    }
    res=mysql_store_result(mysql);
    row=mysql_fetch_row(res);
    if(row==NULL)
    {
        temp->back=-2;
    }
    if((res!=NULL)&&(row!=NULL))
    {
        find_online(mysql,temp);//判断是否在线
    }
}

MYSQL find_online(MYSQL *mysql,struct chat *temp)//查看好友是否在线
{
    printf("temp:%s\n",temp->to_id);
    MYSQL_RES *res;
    MYSQL_ROW *row;
    char query[200]={0};
    char id[16]={0};
    strcpy(id,temp->to_id);
    sprintf(query,"select *from chatuser where id='%s' and sockt='1';",id);
    printf("check_online query:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to query:%s\n",mysql_error(mysql));
    }
    res=mysql_store_result(mysql);
    row=mysql_fetch_row(res);
    if(row==NULL)
    {
        temp->back=-1;
    }
    if(res!=NULL&&row!=NULL)
    {
        temp->back = 1;
    }
 
}

int prin_onlinefriend(MYSQL *mysql,struct chat *temp)
{
    MYSQL_RES *res1;
    MYSQL_ROW *row1;
    char query[200]={0};
    char st[2]={0};
    st[0] = temp->status[2];
    st[1]='\0';
    sprintf(query,"select id,name,sockt from chatuser where sockt='%s' and id IN (select friends from friend where yonghu='%s');",st,temp->user_id);
    printf("\t\tquery1:%s\n\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to query:%s\n",mysql_error(mysql));
    }
    res1=mysql_store_result(mysql);
    //row1=mysql_fetch_row(res1);
    printf("\n");
    /*if(row1 == NULL)
    {
        printf("没有数据\n");
    }*/
    int b=0;
    while((row1=mysql_fetch_row(res1)))
    {
        b++;
        memset(temp->user_name,0,16);
        memset(temp->user_id,0,16);
        memset(temp->to_id,0,16);
        memset(temp->masg,0,16);
        for(t=0;t<mysql_num_fields(res1);t++)
        {
            if(t==0)
            {
                strcpy(temp->to_id,(char *)row1[t]);
            }
            if(t==1)
            {
                strcpy(temp->user_name,(char *)row1[t]);
            }
            if(t==2)
            {
                strcpy(temp->masg,(char *)row1[t]);
            }
        }
        printf("%s\n",temp->to_id);
        printf("%s\n",temp->user_name);
        printf("%s\n",temp->masg);
    }
    if(b==0)
    {
        printf("没有数据\n");
    }
}

MYSQL insert_imformation(MYSQL *mysql,struct chat *temp) //添加消息盒子中的信息
{
    MYSQL *res;
    MYSQL *row;
    char query[200]={0};
    char st[2]={0};
    st[0]=temp->status[1];
    st[1]='\0';
    sprintf(query,"INSERT INTO imformation(s_name, s_id, to_id, masg, type)VALUES('%s', '%s', '%s', '%s', '%s');",temp->user_name,temp->user_id,temp->to_id,temp->masg,st);
    printf("insert_imformation query:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to query:%s\n",mysql_error(mysql));
    }
    res=mysql_store_result(mysql);
    row=mysql_fetch_row(res);
    if(row==NULL)
    {
        temp->back=-1;
    }
    if((res!=NULL)&&(row!=NULL))
    {
        temp->back=1;
    }   
}

int save_friend(MYSQL *mysql,struct chat *temp)
{
    MYSQL *res;
    MYSQL *row;
    char query[200]={0};
    char st[2]={0};
    st[0] = temp->status[0];
    st[1]='\0';
    sprintf(query,"INSERT INTO %s(name,id,S_online,type)VALUES('%s','%s','%s','f');",temp->to_id,temp->user_name,temp->user_id,temp->masg);
    printf("save_friend_query:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to save_friends_ query:%s\n",mysql_error(mysql));
    }
}

int save_mychat(MYSQL *mysql,struct chat *temp)
{
    MYSQL *res;
    MYSQL *row;
    char query[200]={0};
    char st[2]={0};
    sprintf(query,"INSERT INTO %s(name,id,S_online,type)VALUES('I said:','%s','%s','f');",temp->user_id,temp->user_id,temp->masg);
    printf("save_mychat_query:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to save_mychat_ query:%s\n",mysql_error(mysql));
    }

}

/*select s_id,s_name,masg from imformation where to_id='bsc60488' and type='c';*/

/*int delete_friend(MYSQL *mysql,struct chat *temp)
{
    MYSQL_RES *res;
    MYSQL_ROW *row;
    char query[200]={0};
    sprintf(query,"delete from friend where yonghu='%s'")
}*/


MYSQL get_imformation(MYSQL *mysql,struct chat *temp)
{
    MYSQL_RES *res;
    MYSQL_ROW *row;
    char query[200]={0};
    char st[2]={0};
    st[0]=temp->status[1];
    st[1]='\0';
    sprintf(query,"select s_id,s_name,masg from imformation where to_id='%s' and type='%s';",temp->user_id,st);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to query:%s\n",mysql_error(mysql));
    
    }
    res=mysql_store_result(mysql);
    printf("\n");
    while((row=mysql_fetch_row(res)))
    {
        for(t=0;t<mysql_num_fields(res);t++)
        {
            if(t==0){
                strcpy(temp->to_id,(char *)row[t]);
            }
            if(t==1)
            {
                strcpy(temp->user_name,(char *)row[t]);
            }
            if(t==2)
            {
                strcpy(temp->masg,(char *)row[t]);
            }
        }
        printf("%s %s %s\n",temp->to_id,temp->user_name,temp->masg);
        printf("\n");
    }
}



int all_friend(MYSQL *mysql,struct chat *temp)
{
    MYSQL_RES *res1;
    MYSQL_ROW *row1;
    char query[200]={0};
    char st[2]={0};
    st[0] = temp->status[2];
    st[1]='\0';
    sprintf(query,"select id,name,sockt from chatuser where id IN (select yonghu from friend where friends='%s');",temp->user_id);
    printf("\t\tquery1:%s\n\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to query:%s\n",mysql_error(mysql));
    }
    res1=mysql_store_result(mysql);
    printf("\n");
    int b=0;
    while((row1=mysql_fetch_row(res1)))
    {
        b++;
        memset(temp->user_name,0,16);
        memset(temp->to_id,0,16);
        memset(temp->masg,0,16);
        for(t=0;t<mysql_num_fields(res1);t++)
        {
            if(t==0)
            {
                strcpy(temp->to_id,(char *)row1[t]);
            }
            if(t==1)
            {
                strcpy(temp->user_name,(char *)row1[t]);
            }
            if(t==2)
            {
                strcpy(temp->masg,(char *)row1[t]);
            }
        }
        temp->back = 1;
        //save_friend(mysql,temp);
       // write(con_fd,temp,sizeof(struct chat));
        printf("%s ",temp->to_id);
        printf("%s ",temp->user_name);
        printf("%s ",temp->masg);
    }
        all_myfriend(mysql,temp);
}

void chat_history(MYSQL *mysql,struct chat *temp)
{
    MYSQL_RES *res1;
    MYSQL_ROW *row1;
    char query[200]={0};
    char masg[100]={0};
    sprintf(query,"select name,S_online,type from %s where id='%s';",temp->user_id,temp->to_id);
    printf("chat_history_query:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to chat_history_query:%s\n",mysql_error(mysql));
    }
    res1=mysql_store_result(mysql);
    printf("\n");
    int b=0;
    while((row1=mysql_fetch_row(res1)))
    {
        b++;
        for(t=0;t<mysql_num_fields(res1);t++)
        {
            if(t==0)
            {
                strcpy(temp->user_name,(char *)row1[t]);
            }
            if(t==1)
            {
                strcpy(masg,(char *)row1[t]);
            }
            if(t==2)
            {
                //strcat(temp->masg,(char *)row1[t]);
                sprintf(temp->masg,"%s \t\ttime:%s",masg,(char *)row1[t]);
            }
        }
        printf("%s: %s\n",temp->user_name,temp->masg);
    }
    if(b==0)
    {
        printf("\n\t\t\t\t没有聊天记录\n\t\t\t\t");
    }
}
int all_myfriend(MYSQL *mysql,struct chat *temp)
{
    MYSQL_RES *res1;
    MYSQL_ROW *row1;
   // char masg[100];
    char query[200]={0};
    sprintf(query,"select id,name,sockt from chatuser where id IN (select friends from friend where yonghu='%s');",temp->user_id);
    printf("\t\tquery1:%s\n\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to query:%s\n",mysql_error(mysql));
    }
    res1=mysql_store_result(mysql);
    printf("\n");
    int b=0;
    while((row1=mysql_fetch_row(res1)))
    {
        b++;
        for(t=0;t<mysql_num_fields(res1);t++)
        {
            if(t==0)
            {
                strcpy(temp->to_id,(char *)row1[t]);
            }
            if(t==1)
            {
                strcpy(temp->user_name,(char *)row1[t]);
            }
            if(t==2)
            {
                strcpy(temp->masg,(char *)row1[t]);
               // sprintf(temp->masg,"%s %s",masg,(char *)row1[t]);
            }
        }
        temp->back = 1;
        //save_friend(mysql,temp);
       // write(con_fd,temp,sizeof(struct chat));
        printf("%s ",temp->to_id);
        printf("%s ",temp->user_name);
        printf("%s \n",temp->masg);
    }
    if(b==0)
    {
        temp->back = -1;
        //write(con_fd,temp,sizeof(struct chat));
        printf("没有数据\n");
    }
}

/*void check_imformation(MYSQL *mysql,struct chat *temp)
{
    MYSQL_RES *res1;
    MYSQL_ROW *row1;
    char query[200]={0};
    sprintf(query,"select *")
}*/
int deletefriends(MYSQL *mysql,struct chat *temp)
{
    MYSQL *res;
    MYSQL *row;
    char query[200]={0};
    sprintf(query,"delete from friend where yonghu='%s' and friends='%s';",temp->user_id,temp->to_id);
    printf("chat_history_query:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to chat_history_query:%s\n",mysql_error(mysql));
    }
}
int log(MYSQL *mysql,struct chat *temp,char *sprin)
{
    MYSQL *res;
    MYSQL *row;
    char query[200]={0};
    sprintf(query,"INSERT INTO log (name,id,command,time)VALUES(%s,%s,%s,%s)",temp->user_name,temp->user_id,sprin)
}
int main()
{
    MYSQL *mysql;
    struct chat temp;
    char *id="bsc21963";
    char *name="ljdf";
    char *to_id="bsc30000";
    char *masg="你好";
    memset(&temp,0,sizeof(struct chat));
    strcpy(temp.to_id,to_id);
    strcpy(temp.user_name,name);
    strcpy(temp.user_id,id);
    strcpy(temp.masg,masg);
    temp.status[2]='1';
    //printf("id=%s\n",temp.user_id);
    temp.back = 0;
    my_sql_init(mysql);
    mysql_connect(mysql);
    //find_reg(mysql,&temp);
    //insert_imformation(mysql,&temp);
   // get_imformation(mysql,&temp);
   // prin_onlinefriend(mysql,&temp);
   // save_friend(mysql,&temp);
   // save_mychat(mysql,&temp);
    //chat_history(mysql,&temp);
   // all_friend(mysql,&temp);
    //all_myfriend(mysql,&temp);
   /* printf("flag = %d\n",temp.back);
    if(temp.back == 1)
    {
        temp.command = 1;
        temp.back = 1;
    }*/
    //deletefriends(mysql,&temp);
    //printf("back:%d\n",temp.back);

}
