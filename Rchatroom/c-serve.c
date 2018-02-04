/*************************************************************************
	> File Name: serve.c
	> Author: hujialu
	> Mail: 
> Created Time: 2017年08月16日 星期三 14时48分59秒
************************************************************************/

#include"data.h"
#define HOST "localhost"
#define USER "root"
#define PASSWD " "
#define DB_NAME "chatroom"
MYSQL find_online(MYSQL *mysql,struct chat *temp);//查看好友是否在线
int online_flag(MYSQL *mysql,struct chat *temp)//上下线改变状态
{
char query[200]={0};
printf("1status[0]:%d\n",temp->status[0]);
int choice = temp->status[0];
/*update chat_user set s_online='1' where id='bsc90526';*/
memset(query,0,sizeof(query));
if(choice==0){
temp->status[0] = 1;
printf("status:1\n");
sprintf(query,"update chatuser set sockt='1' where id='%s';",temp->user_id);}
if(choice==1)
{
temp->status[0] = 0;
printf("status:0\n");
sprintf(query,"update chatuser set sockt='0' where id='%s';",temp->user_id);}
MYSQL *res;
MYSQL *row;
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
printf("Falied to query:%s\n",mysql_error(mysql));
return -1;
}
printf("query44:%s\n",query);
printf("successful of change the status\n");
return 1;
}
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
if(!mysql_set_character_set(mysql,"utf8"))
{
printf("new client charact set :%s\n",mysql_character_set_name(mysql));
}
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
char *command="select *from chatuser where";
sprintf(query,"%s passwd='%s' and id='%s';",command,temp->user_passwd,temp->user_id);
printf("query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
printf("Falied to query:%s\n",mysql_error(mysql));
}
res = mysql_store_result(mysql);
printf("\n");
int flag=0;
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
    if(t==3)//已经登陆
    {
        char st[2];
        st[0]='1';
        st[1]='\0';
        if(!strcmp(st,(char *)row[t]))
        {
            flag = 0;//该用户在线
            return 0;
        }
        printf("status:%s\n",(char *)row[t]);
    }
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
//temp->back=1;
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
char *lie="(name char(50) NULL, id char(16) NOT NULL, S_online char(200) NOT NULL, type char(200))ENGINE=InnoDB DEFAULT CHARSET=utf8;";
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

int create_circles(MYSQL *mysql,struct chat *temp)
{
MYSQL *res;
MYSQL *row;
char query[200]={0};
sprintf(query,"CREATE TABLE IF NOT EXISTS %s (name char(20) NOT NULL, id char(16) NULL,masg char(200) NULL,type char(200) NULL)ENGINE=InnoDB DEFAULT CHARSET=utf8;",temp->to_id);
printf("create_circle_query:%s\n",query);
int t = mysql_real_query(mysql, query, strlen(query));
if(t)
{
    printf("Falied to create_circle_query:%s\n",mysql_error(mysql));
    return 0;
}
return 1;
}
int outto_circle(MYSQL *mysql,struct chat *temp)
{
MYSQL *res;
MYSQL *row;
char query[200]={0};
sprintf(query,"delete from circle where user_id='%s' and circle_id='%s';",temp->user_id,temp->to_id);
printf("delete_circle_query:%s\n",query);
int t = mysql_real_query(mysql, query, strlen(query));
if(t)
{
    printf("Falied to create_circle_query:%s\n",mysql_error(mysql));
    return 0;
}
return 1;
}

int insert_circledata(MYSQL *mysql,struct chat *temp)
{
MYSQL *res;
MYSQL *row;
char query[300]={0};
char st[2]={0};
st[0] = temp->status[2];
st[1] = '\0';
sprintf(query,"INSERT INTO circle (user_id,circle_id,circle_name,type)VALUES('%s','%s','%s','%s');",temp->user_id,temp->to_id,temp->masg,st);
printf("inser_circle_query:%s\n",query);
int t = mysql_real_query(mysql, query, strlen(query));
if(t)
{
    printf("Falied to intsert_circledata_query:%s\n",mysql_error(mysql));
    return 0;
}
return 1;
}

int look_circles(MYSQL *mysql,struct chat *temp,int con_fd)
{
MYSQL_RES *res;
MYSQL_ROW *row;
char query[200]={0};
sprintf(query,"select circle_name,circle_id from circle where user_id='%s';",temp->user_id);
printf("look_circles_query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to query:%s\n",mysql_error(mysql));
}
res=mysql_store_result(mysql);
printf("\n");
int b=0; 
while((row=mysql_fetch_row(res)))
{
    b++;
    for(t=0;t<mysql_num_fields(res);t++)
    {
        if(t==0)
        {
            strcpy(temp->user_name,(char *)row[t]);
        }
        if(t==1)
        {
            strcpy(temp->to_id,(char *)row[t]);
        }
    }
    temp->back = 1;
    write(con_fd,temp,sizeof(struct chat));
}
if(b==0)
{
    temp->back = -1;
    write(con_fd,temp,sizeof(struct chat));
    //return 0;
}
}

int host_circle(MYSQL *mysql,struct chat *temp)
{
MYSQL_RES *res;
MYSQL_ROW *row;
char query[200]={0};
char id[16]={0};
sprintf(query,"select user_id from circle where type='h' and circle_id='%s';",temp->to_id);
printf("host_circle_query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to query:%s\n",mysql_error(mysql));
}
res=mysql_store_result(mysql);
printf("\n");
int b=0; 
while((row=mysql_fetch_row(res)))
{
    b++;
    for(t=0;t<mysql_num_fields(res);t++)
    {
        strcpy(id,(char *)row[t]);
    }
}
if(b==0)
{
    return 0;
}
else
{
    if(strcmp(id,temp->user_id)==0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

}
int close_circles(MYSQL *mysql,struct chat *temp)//解散群
{
MYSQL *res;
MYSQL *row;
char query[200]={0};
sprintf(query,"delete from circle where circle_id='%s';",temp->to_id);
printf("close_circles_ query:%s\n",query);
int t = mysql_real_query(mysql, query, strlen(query));
if(t)
{
    printf("Falied to intsert_circledata_query:%s\n",mysql_error(mysql));
    return 0;
}
return 1;
}
int check_addcircle(MYSQL *mysql,struct chat *temp)//检查是否有该群
{
MYSQL_RES *res;
MYSQL_ROW *row;
char query[200]={0};
sprintf(query,"select circle_name from circle where circle_id='%s';",temp->to_id);
printf("check_online query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to query:%s\n",mysql_error(mysql));
}
res=mysql_store_result(mysql);
printf("\n");
int b=0; 
while((row=mysql_fetch_row(res)))
{
    for(t=0;t<mysql_num_fields(res);t++)
    {
        strcpy(temp->masg,(char *)row[0]);
        printf("\t\t\t\tname for circle:%s\n\t\t\t",temp->masg);
        b++;
    }
    //b++;
}
if(b==0)
{
    return 0;
}
if(b>0)
{
    return 1;
}
}

int check_personcircle(MYSQL *mysql,struct chat *temp)//检查是否有重复加群
{
MYSQL_RES *res;
MYSQL_ROW *row;
char query[200]={0};
sprintf(query,"select user_id from circle where circle_id='%s' and user_id='%s';",temp->to_id,temp->user_id);
printf("check_online query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to query:%s\n",mysql_error(mysql));
}
res=mysql_store_result(mysql);
printf("\n");
int b=0; 
while((row=mysql_fetch_row(res)))
{
    for(t=0;t<mysql_num_fields(res);t++)
    {
        b++;
    }
    //b++;
}
if(b==0)
{
    return 1;
}
if(b>0)
{
    return 0;
}
}
int allchat(MYSQL *mysql,struct chat *temp,int con_fd,struct connect *phead)
{
MYSQL_RES *res;
MYSQL_ROW *row;
char query[200]={0};
int conn;
char id[16]={0};
sprintf(query,"select id from chatuser where sockt='1' and id IN(select user_id from circle where circle_id='%s');",temp->to_id);
printf("allchat_query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to query:%s\n",mysql_error(mysql));
}
res=mysql_store_result(mysql);
printf("\n");
int b=0;
while((row=mysql_fetch_row(res)))
{
    b++;
    for(t=0;t<mysql_num_fields(res);t++)
    {
        strcpy(id,(char *)row[t]);
        if(strcmp(temp->user_id,id)!=0)
        {
            conn = prin(phead,id);
            temp->command = all_chat;
            temp->back = 1;
            write(conn,temp,sizeof(struct chat));
            printf("\n\t\t\t\txiaxi:%s\n",temp->masg);
        }
    }
}
//if(b>0)
// {
    send_allchat(mysql,temp);
// }
if(b==0)
{
    temp->command = all_chat;
    temp->back = -1;
    write(con_fd,temp,sizeof(struct chat));
}
}
int look_allchat_history(MYSQL *mysql,struct chat *temp,int con_fd)//查看群聊天记录
{
MYSQL_RES *res;
MYSQL_ROW *row;
char query[200]={0};
char masg[200]={0};
sprintf(query,"select name,masg,type from %s;",temp->to_id);
printf("look_allchat_history_query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to query:%s\n",mysql_error(mysql));
}
res=mysql_store_result(mysql);
printf("\n");
int b=0;
while((row=mysql_fetch_row(res)))
{
    b++;
    for(t=0;t<mysql_num_fields(res);t++)
    {
        if(t==0)
        {
            strcpy(temp->user_name,(char *)row[t]);
        }
        if(t==1)
        {
            strcpy(masg,(char *)row[t]);
        }
        if(t==2)
        {
            sprintf(temp->masg,"%s time:%s",masg,(char *)row[t]);
        }
    }
    temp->command = look_circle_history;
    temp->back = 1;
    write(con_fd,temp,sizeof(struct chat));
}
if(b==0)
{
    temp->command = look_circle_history;
    temp->back = -1;
    write(con_fd,temp,sizeof(struct chat));
}
}
int send_allchat(MYSQL *mysql,struct chat *temp)
{
time_t timer;
struct tm *tblock;
timer = time(NULL);
tblock = localtime(&timer);
MYSQL *res;
MYSQL *row;
char query[200]={0};
sprintf(query,"INSERT INTO %s (name,id,masg,type)VALUES('%s','%s','%s','%s');",temp->to_id,temp->user_name,temp->user_id,temp->masg,asctime(tblock));
printf("send_allchat_query:%s\n",query);
int t = mysql_real_query(mysql, query, strlen(query));
if(t)
{
    printf("Falied to query:%s\n",mysql_error(mysql));
    return 0;
}
return 1;
}

MYSQL *insert_data(MYSQL *mysql,struct chat *temp)//向用户表中插入数据
{
char *s1="INSERT INTO chatuser(name, id, passwd, sockt)VALUES(";
char query[200];
char *end=");";
MYSQL *res;
MYSQL *row;
char st[2];
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


MYSQL find_reg(MYSQL *mysql,struct chat *temp) //判断是否有该好友
{
printf("temp@@@:%d\n",temp->back);
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
printf("myouhaoyou\n");
temp->back=-1;
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
temp->back=-2;//不在线
}
if((res!=NULL)&&(row!=NULL))
{
temp->back=1;//在线
}   
}

MYSQL insert_imformation(MYSQL *mysql,struct chat *temp) //插入消息盒子列表
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
}


MYSQL get_imformation(MYSQL *mysql,struct chat *temp,int con_fd)//从表中获取消息
{
MYSQL_RES *res;
MYSQL_ROW *row;
char query[200]={0};
char st[2]={0};
st[0]=temp->status[1];
st[1]='\0';
sprintf(query,"select s_id,s_name,masg from imformation where to_id='%s' and type='%s';",temp->user_id,st);
printf("get_imformation query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
printf("Falied to query:%s\n",mysql_error(mysql));
}
res=mysql_store_result(mysql);
printf("\n");
int b=0; 
while((row=mysql_fetch_row(res)))
{
b++;
temp->back = 1;
memset(temp->to_id,0,16);
memset(temp->masg,0,200);
memset(temp->user_name,0,16);
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
write(con_fd,temp,sizeof(struct chat));
printf("%s %s %s\n",temp->to_id,temp->user_name,temp->masg);
printf("\n");
}
if(b==0)
{
temp->back = -1;
write(con_fd,temp,sizeof(struct chat));
}

}

int insert_intofriend(MYSQL *mysql,struct chat *temp)//确定好友关系后往其中添加好友关系表
{
MYSQL *res;
MYSQL *row;
char query[200];
sprintf(query,"INSERT INTO friend (yonghu, friends)VALUES('%s', '%s');",temp->user_id,temp->to_id);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
printf("Falied to query:%s\n",mysql_error(mysql));}
return 1;
}


int check_refriend(MYSQL *mysql,struct chat *temp)//加好友前的判断
{
MYSQL_RES *res;
MYSQL_ROW *row;
char query[200]={0};
int flag = 0;
sprintf(query,"select *from friend;");
printf("check_refriend_query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to query:%s\n",mysql_error(mysql));
}
if(strcmp(temp->to_id,temp->user_id)==0)//不能加自己
{
    return -2;
}
res=mysql_store_result(mysql);
printf("\n");
while((row=mysql_fetch_row(res)))
{
    flag = 0;
    for(t=0;t<mysql_num_fields(res);t++)
    {
        if((strcmp((char *)row[t],temp->user_id)==0)||(strcmp((char *)row[t],temp->to_id)==0))
        {
            printf("%d\n",flag);
            flag = flag+1;
            system("clear -l");
            printf("flag::%d\n",flag);
            if(flag == 2)
            {
                break;
            }
            continue;
        }
        else
        {
            flag = 0;
            continue;
        }
    }
}
if(flag==2)
{
    return 1;//存在该好友
}
else
{
    return -1; //不存在该好友
}
}

int prin_onlinefriend(MYSQL *mysql,struct chat *temp,int con_fd)
{
//temp->command = look_online;
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
printf("\n");
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
    temp->back = 1;
    write(con_fd,temp,sizeof(struct chat));
    printf("%s ",temp->to_id);
    printf("%s ",temp->user_name);
    printf("%s ",temp->masg);
}
    prin_linefriend(mysql,temp,con_fd,b);
}


int prin_linefriend(MYSQL *mysql,struct chat *temp,int con_fd,int flag)
{
//temp->command = look_online;
MYSQL_RES *res1;
MYSQL_ROW *row1;
char query[200]={0};
char st[2]={0};
st[0] = temp->status[2];
st[1]='\0';
sprintf(query,"select id,name,sockt from chatuser where sockt='%s' and id IN (select yonghu from friend where friends='%s');",st,temp->user_id);
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
    temp->back = 1;
    write(con_fd,temp,sizeof(struct chat));
    printf("%s ",temp->to_id);
    printf("%s ",temp->user_name);
    printf("%s ",temp->masg);
}
 if(b==0&&flag==0)
{
    temp->back = -1;
    write(con_fd,temp,sizeof(struct chat));
    printf("没有数据\n");
}
}

int save_onlinefriend(MYSQL *mysql,struct chat *temp)
{
MYSQL_RES *res1;
MYSQL_ROW *row1;
char query[200]={0};
/*char st[2]={0};
st[0] = temp->status[0];
st[1]='\0';*/
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
    save_friend(mysql,temp);
    printf("%s ",temp->to_id);
    printf("%s ",temp->user_name);
    printf("%s ",temp->masg);
}
if(b==0)
{
    temp->back = -1;
    //write(con_fd,temp,sizeof(struct chat));
    printf("没有数据\n");
}
}


int save_linefriend(MYSQL *mysql,struct chat *temp)
{
MYSQL_RES *res1;
MYSQL_ROW *row1;
char query[200]={0};
/*char st[2]={0};
st[0] = temp->status[2];
st[1]='\0';*/
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
    char name[16]={0};
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
            strcpy(name,(char *)row1[t]);
        }
        if(t==2)
        {
            strcpy(temp->masg,(char *)row1[t]);
        }
    }
    save_friend(mysql,temp,name);
    printf("%s ",temp->to_id);
    printf("%s ",temp->user_name);
    printf("%s ",temp->masg);
}
/* if(b==0)
{
    temp->back = -1;
    write(con_fd,temp,sizeof(struct chat));
    printf("没有数据\n");
}*/
}


int save_friend(MYSQL *mysql,struct chat *temp,char *name)//保存好友信息
{
MYSQL *res;
MYSQL *row;
char query[200]={0};
char st[2]={0};
st[0] = temp->status[2];
st[1]='\0';
sprintf(query,"INSERT INTO %s(name,id,S_online,type)VALUES('%s','%s','%s','%s');",temp->user_id,name,temp->to_id,st,st);
printf("save_friend_query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to query:%s\n",mysql_error(mysql));
}
}
int save_friends(MYSQL *mysql,struct chat *temp)//保存好友信息
{
MYSQL *res;
MYSQL *row;
char query[200]={0};
char st[2]={0};
st[0] = temp->status[2];
st[1]='\0';
sprintf(query,"INSERT INTO %s(name,id,S_online,type)VALUES('%s','%s','%s','%s');",temp->to_id,temp->user_name,temp->user_id,st,st);
printf("save_friend_query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to query:%s\n",mysql_error(mysql));
}
}
/*int prin_linefriend(MYSQL *mysql,struct chat *temp,int con_fd)
{
//temp->command = look_online;
MYSQL_RES *res1;
MYSQL_ROW *row1;
char query[200]={0};
char st[2]={0};
st[0] = temp->status[2];
st[1]='\0';
sprintf(query,"select id,name,sockt from chatuser where sockt='%s' and id IN (select yonghu from friend where friends='%s');",st,temp->user_id);
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
    temp->back = 1;
    //save_friend(mysql,temp);
    write(con_fd,temp,sizeof(struct chat));
    printf("%s ",temp->to_id);
    printf("%s ",temp->user_name);
    printf("%s ",temp->masg);
}
/if(b==0)
{
    temp->back = -1;
    write(con_fd,temp,sizeof(struct chat));
    printf("没有数据\n");
}/
}*/

int all_friend(MYSQL *mysql,struct chat *temp,int con_fd)
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
    write(con_fd,temp,sizeof(struct chat));
    printf("%s ",temp->to_id);
    printf("%s ",temp->user_name);
    printf("%s ",temp->masg);
}
    all_myfriend(mysql,temp,con_fd,b);
}


int all_myfriend(MYSQL *mysql,struct chat *temp,int con_fd,int flag)
{
MYSQL_RES *res1;
MYSQL_ROW *row1;
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
        }
    }
    temp->back = 1;
    write(con_fd,temp,sizeof(struct chat));
    printf("%s ",temp->to_id);
    printf("%s ",temp->user_name);
    printf("%s \n",temp->masg);
}
if(b==0&&flag==0)
{
    temp->back = -3;
    write(con_fd,temp,sizeof(struct chat));
    printf("没有数据\n");
}
}


int save_friendchat(MYSQL *mysql,struct chat *temp)
{
time_t timer;
struct tm *tblock;
timer = time(NULL);
tblock = localtime(&timer);
MYSQL *res;
MYSQL *row;
char query[200]={0};
char st[2]={0};
st[0] = temp->status[0];
st[1]='\0';
sprintf(query,"INSERT INTO %s(name,id,S_online,type)VALUES('%s:','%s','%s','%s');",temp->to_id,temp->user_name,temp->user_id,temp->masg,asctime(tblock));
printf("save_friend_query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to save_friends_ query:%s\n",mysql_error(mysql));
}
}

int save_mychat(MYSQL *mysql,struct chat *temp)
{
time_t timer;
struct tm *tblock;
timer = time(NULL);
tblock = localtime(&timer);
MYSQL *res;
MYSQL *row;
char query[200]={0};
char st[2]={0};
sprintf(query,"INSERT INTO %s(name,id,S_online,type)VALUES('I said:','%s','%s','%s');",temp->user_id,temp->to_id,temp->masg,asctime(tblock));
printf("save_mychat_query:%s\n",query);
int t = mysql_real_query(mysql,query,strlen(query));
if(t){
    printf("Falied to save_mychat_ query:%s\n",mysql_error(mysql));
}

}

void chat_history(MYSQL *mysql,struct chat *temp,int con_fd)
{
    temp->command = person_chat;
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
        temp->back = 4;
        write(con_fd,temp,sizeof(struct chat));
    }
    if(b==0)
    {
        temp->back = -4;//没有聊天记录
        printf("\n\t\t\t\t没有聊天记录\n\t\t\t\t");
        write(con_fd,temp,sizeof(struct chat));
    }
}

int delete_friends(MYSQL *mysql,struct chat *temp)
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
int deletefriends(MYSQL *mysql,struct chat *temp)
{
    MYSQL *res;
    MYSQL *row;
    char query[200]={0};
    sprintf(query,"delete from friend where yonghu='%s' and friends='%s';",temp->to_id,temp->user_id);
    printf("chat_history_query:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to chat_history_query:%s\n",mysql_error(mysql));
    }
}



int logs(MYSQL *mysql,struct chat *temp,char *sprin)
{
time_t timer;
struct tm *tblock;
timer = time(NULL);
tblock = localtime(&timer);
    MYSQL *res;
    MYSQL *row;
    char query[200]={0};
    sprintf(query,"INSERT INTO log (name,id,command,time)VALUES('%s','%s','%s','%s')",temp->user_name,temp->user_id,sprin,asctime(tblock));
    printf("log_sprin:%s\n",query);
    int t = mysql_real_query(mysql,query,strlen(query));
    if(t){
        printf("Falied to chat_history_query:%s\n",mysql_error(mysql));
    }

}
