/*************************************************************************
	> File Name: ceshi2.c
	> Author: 
	> Mail: 
	> Created Time: 三  8/ 9 21:12:12 2017
 ************************************************************************/
#include"data.h"
#include<pthread.h>
#include"serve.h"
#define LISTENQ 12
MYSQL *mysql;
struct connect *insert(struct connect *phead,struct chat *temp,int conn_fd);
void apply_addfriend(int conn_fd,struct chat *temp);//处理申请加好友
void send_friend(int conn_fd,struct chat *temp);//处理申请加好友

struct connect
{
   int connect_id;
   char connect_userid[16];
   struct connect *next;
};
struct connect *phead;
struct connect *p;
struct connect *q;
struct connect *r;
time_t timer;
struct tm *tblock;

char *ch="bsc";
char friend[200][200]={0};

int prin(struct connect *phead,char *id)//查找在线节点conn_fd;
{
    int a;
    struct connect *p = phead->next;
    while(p!=NULL)
    {
        if(!strcmp(p->connect_userid,id))
        {
            a = p->connect_id;  
            return a;
            break;
        }
        p = p->next;
    }
    return -1;
}

struct connect *online_prin(struct connect *phead,char *id)//退出删除节点
{
    struct connect *p = phead;
    struct connect *pre = phead;
    if(p==NULL)
    {
        return phead;
    }
    while(p != NULL)
    {
        if(!strcmp(p->connect_userid,id))
        {
            pre->next = p->next;
            free(p);
            break;
        }
        pre = p;
        p = p->next;
    }
    return phead;
}

struct connect *insert(struct connect *phead,struct chat *temp,int conn_fd)
{
    struct connect *p=phead;
    struct connect *r,*new;
    new = (struct connect *)malloc(sizeof(struct connect));
    new->connect_id = conn_fd;//记录每次登陆连接的套接字
    strcpy(new->connect_userid,temp->user_id);//为之后的用户Id做准备
    if(p==NULL)
    {
        p->next = new;
        p=new;
        new->next = NULL;
    }
    while(p!=NULL)
    {
        r = p;
        p = p->next;
    }
    r->next = new;
    new->next = NULL;
    return phead;
}
void apply_addfriend(int conn_fd,struct chat *temp)//处理申请加好友
{
    if(temp->back==1)/*有该好友并在线*/
    {
        char id[16]={0};
        int conn;
        strcpy(id,temp->to_id);
        conn=prin(phead,id);
        printf("id:%s\n",id);
        printf("conn:%d\n",conn);
        temp->command = add_friend;
        temp->back = 1;
        printf("\t\t\t\t\t\tname:%s\t\t",temp->user_name);
        printf("\t\t\t\t\t\tid:%s\t\t",temp->user_id);
        printf("\t\t\t\t\t\tmasg:%s\t\t",temp->masg);
        int a = write(conn,temp,sizeof(struct chat));
        printf("a=%d\n",a);
    }
    if(temp->back == -2)/*有该好友但是不在线*/
    {
        temp->command = add_friend;
        temp->back = -2;
        int a = write(conn_fd,temp,sizeof(struct chat));
        printf("a=5 %d\n",a);
        printf("back = -2");
    }
    if(temp->back==-1)/*该用户没有注册*/
    {
        temp->command = add_friend;
        temp->back = -1;
        int a = write(conn_fd,temp,sizeof(struct chat));
        printf("a=6 %d\n",a);
        printf("back =-1");
    }
    temp->status[1]='c';
}
void send_friend(int conn_fd,struct chat *temp)//处理私聊信息
{
    if(temp->back==1)/*有该好友并在线*/
    {
        char id[16]={0};
        int conn;
        strcpy(id,temp->to_id);
        conn=prin(phead,id);
        printf("id:%s\n",id);
        printf("conn:%d\n",conn);
        temp->command = person_chat;
        temp->back = 3;
        //while(1){
        printf("\t\t\t\t\t\tname:%s\t\t",temp->user_name);
        printf("\t\t\t\t\t\tid:%s\t\t",temp->user_id);
        printf("\t\t\t\t\t\tmasg:%s\t\t",temp->masg);
        int a = write(conn,temp,sizeof(struct chat));
        printf("a=%d\n",a);
        //}
    }
    if(temp->back == -2)/*有该好友但是不在线*/
    {
        temp->command = person_chat;
        temp->back = -2;
        temp->status[1]='f';
        int a = write(conn_fd,temp,sizeof(struct chat));
        insert_imformation(mysql,temp);
        printf("back = -2,lixian liaotian\n");
    }
    if(temp->back==-1)/*该用户没有注册*/
    {
        temp->command = person_chat;
        temp->back = -1;
        int a = write(conn_fd,temp,sizeof(struct chat));
        printf("a=6 %d\n",a);
        printf("back =-1");
    }
   // temp->status[1]='f';
}

void *dosome(void *arg)
{
    int conn_fd=(int)arg;
    stchat temp;
    char *Id;
    char send_Id[20]={0};
    Id = (char *)malloc(16);
    int flag=1;//登陆成功将退出
    int cho=0;
    while(flag){
        memset(&temp,0,sizeof(temp));
        int n = read(conn_fd,&temp,sizeof(stchat));
        if(n==0)
        {
            printf("statius:%d\n",temp.status[0]);
            if(cho==1)
            {
                printf("该用户退出注册\n");
                break;
            }
            else{
            printf("用户已断开连接\n");
            pthread_exit("有用户退出");
            }
        }
        if(n>0){
                /*mysql = my_sql_init(mysql);
                mysql = mysql_connect(mysql);*/
                printf("passwd:%s\n",temp.user_passwd);
                int choise=temp.command;
                printf("command=%d\n",temp.command);
                switch(choise)
                {
                    case 1:
                    {
                        cho = 1;
                        printf("zhuze\n");
                        cho=1;
                        /*产生随机账号*/
                        srand((unsigned)time(NULL));
                        char id[12];
                        char st[10];
                        for(int i=0;i<5;i++)
                        {
                            st[i] = (rand()%10);
                            id[i] = st[i]+48;
                        }
                        id[5]='\0';
                        int a=1;
                        char *sprin="reg the ID successful";
                        /*随机账号前加入字符*/
                        sprintf(send_Id,"%s%s",ch,id);
                        printf("a=%s\n",send_Id);
                        strcpy(temp.user_id,send_Id);
                        temp.back = 1;
                        a = write(conn_fd,&temp,sizeof(struct chat));
                        printf("a=%d\n",a);
                        if(a <= 0)
                        {
                            printf("cuowu\n");
                        }
                        else
                        {
                            printf("产生随机账号:%s\n",id);
                            mysql = insert_data(mysql,&temp);
                            mysql = create_table(mysql,send_Id);
                            printf("返回账号成功!\n");
                            logs(mysql,&temp,sprin);
                        }
                        break;
                    }
                    case 2://处理登陆
                    {
                        int Login =0;//
                        Login = login_find(mysql,&temp);
                        printf("Login=%d\n",Login);
                        if(Login==0)
                        {
                            char *sprin="重复登陆";
                            temp.back=0;//账号已经登陆过
                            logs(mysql,&temp,sprin);
                        }
                        if(Login==1)//成功登陆
                        {
                            char *sprin="登陆成功";
                            flag = 0;
                            temp.status[0]=0;
                            int b = online_flag(mysql,&temp);
                            phead = insert(phead,&temp,conn_fd);
                            printf("b=%d\n",b);
                            temp.back=1;
                            logs(mysql,&temp,sprin);
                        }
                        if(Login==-1)
                        {
                            char *sprin="密码错误";
                            temp.back=-1;
                            logs(mysql,&temp,sprin);
                        }
                        if(Login==-2)
                        {
                            char *sprin="账号错误";
                            temp.back = -2;
                            logs(mysql,&temp,sprin);
                        }
                        write(conn_fd,&temp,sizeof(struct chat));
                    printf("two\n");
                    break;
                }
            }
        }
}
    struct connect *q=phead->next;
    while(q!=NULL)
    {
        printf("q:%d\n",q->connect_id);
        q=q->next;
    }
    printf("temp:\n");
    printf("temp.user_passwd:%s\n",temp.user_passwd);
    printf("temp.user_id:%s\n",temp.user_id);
    printf("temp.user_name:%s\n",temp.user_name);
    printf("temp.command:%d\n",temp.command);
    printf("temp.status:%d\n",temp.status[0]);
    printf("temp.status:%d\n",temp.status[1]);
    printf("temp.back:%d\n",temp.back);
    printf("temp.to_id:%s\n",temp.to_id);
    int End=1;
    printf("temp.back&&&&:%d\n",temp.back);
    while(End)
    {
        int n = read(conn_fd,&temp,sizeof(struct chat));
        temp.back=0;
        printf("temp.user_id:%s\n",temp.user_id);
        printf("temp.back****:%d\n",temp.back);
        printf("temp.command:%d\n",temp.command);
       // printf("temp.lens:%d\n",temp.lens);
       // printf("标记:%c",temp.status[2]);
        if(n==0)
        {
            char id[16]={0};
            char *sprin="用户退出";
            logs(mysql,&temp,sprin);
            strcpy(id,temp.user_id);
            phead = online_prin(phead,id);//退出删除节点
            int b = online_flag(mysql,&temp);
            printf("有用户退i出\n");
            memset(&temp,0,sizeof(struct chat));
            End = 0;
        }
        if(n<0)
        {
            printf("发生错误\n");
        }
        else{
                switch(temp.command)//分析客户端的命令请求
                {
                    printf("\t\t\t\temp.command:%d**###@@@\n",temp.command);
                    case 1://加好友处理
                    {
                       // char *sprin="加好友请求";
                        char sprin[200]={0};
                        sprintf(sprin,"请求%s加为好友",temp.to_id);
                        logs(mysql,&temp,sprin);
                        logs(mysql,&temp,sprin);
                        int flag = check_refriend(mysql,&temp);
                        printf("\n\n\nflag=%d\n\n\n",flag);
                        if(flag == -1){//没有该好友
                            printf("加好友处理请求\n");
                            //printf("temp.back:%d\n",temp.back);
                            find_reg(mysql,&temp);
                            //printf("temp.back:%d\n",temp.back);
                            apply_addfriend(conn_fd,&temp);
                            insert_imformation(mysql,&temp);}
                        if(flag == 1)//存在该好友
                        {
                            temp.back = add_friend;
                            temp.back =-3;
                            write(conn_fd,&temp,sizeof(struct chat));
                        }
                        if(flag == -2)//输入ID为自己ID
                        {
                            temp.command = add_friend;
                            temp.back = -4;
                            write(conn_fd,&temp,sizeof(struct chat));
                        }
                        break;
                    }
                    case 2: //删除好友
                    {
                        char sprin[200]={0};
                        sprintf(sprin,"请求%s加为好友",temp.to_id);
                        logs(mysql,&temp,sprin);
                        printf("\t\t\t\tdelete friends\n\t\t\t\t");
                        delete_friends(mysql,&temp);
                        deletefriends(mysql,&temp);
                        temp.command = delete_friend;
                        temp.back = 1;
                        write(conn_fd,&temp,sizeof(struct chat));
                        break;
                    }
                    case 4: //查看在线好友
                    {
                        char *sprin="查看在线好友";
                        logs(mysql,&temp,sprin);
                        printf("在线好友查看\n");
                        temp.command = look_online;
                       // all_friend(mysql,&temp,conn_fd);
                        prin_onlinefriend(mysql,&temp,conn_fd);
                        printf("98797\n");
                        break;
                    }
                    case 5: //查看离线好友
                    {
                        char *sprin="查看离线好友";
                        logs(mysql,&temp,sprin);
                        printf("在线好友查看\n");
                        printf("离线好友查看\n");
                        temp.command = look_outline;
                        prin_onlinefriend(mysql,&temp,conn_fd);
                        //prin_linefriend(mysql,&temp,conn_fd);
                        printf("98796\n");
                        break;
                    }
                    case 6://进入私聊
                    {
                        printf("jinru silioa\n");
                        if(temp.status[2]=='0')//拉取好友列表
                        {
                        char *sprin="查看好友列表";
                        logs(mysql,&temp,sprin);
                        printf("在线好友查看\n");
                            temp.command = person_chat;
                            temp.back = 1;
                            all_friend(mysql,&temp,conn_fd);
                            printf("laquhaoyoulianbiao\n");   
                        }
                        if(temp.status[2]=='1')//输入ID进行私聊
                        {
                            printf("AAliaotian\n");
                            int flag = check_refriend(mysql,&temp);
                            printf("flag:%d\n",flag);
                            if(flag == 1)//存在该好友
                            {
                                char sprin[200]={0};
                                sprintf(sprin,"与%s进行私聊",temp.to_id);
                                logs(mysql,&temp,sprin);
                                temp.command = person_chat;
                                temp.back = 2;
                                find_reg(mysql,&temp);
                                printf("\t\t\t\t$$^^temp.back=%d",temp.back);
                               send_friend(conn_fd,&temp);//处理申请加好友
                               // insert_imformation(mysql,&temp);
                                save_friendchat(mysql,&temp);
                                save_mychat(mysql,&temp);
                            }
                            else
                            {
                                char sprin[200]={0};
                                sprintf(sprin,"私聊未找到匹配好友%s",temp.to_id);
                                logs(mysql,&temp,sprin);
                                temp.command = person_chat;
                                temp.back = -1;//没有该好友
                                write(conn_fd,&temp,sizeof(struct chat));
                            }
                            printf("liaotian\n");
                        }
                        if(temp.status[2]=='2')
                        {
                            char sprin[200]={0};
                            sprintf(sprin,"查看与%s的聊天记录",temp.to_id);
                            logs(mysql,&temp,sprin);
                            chat_history(mysql,&temp,conn_fd);
                           // write(conn_fd,&temp,sizeof(struct chat));
                        }
                        break;

                    }
                    case 7: //创建群
                    {
                        int c;
                        int b;
                        temp.command = create_circle;
                        printf("\n\t\t\t\tCreated circle\n");
                        srand((unsigned)time(NULL));
                        char id[12];
                        char st[10];
                        for(int i=0;i<5;i++)
                        {
                            st[i] = (rand()%10);
                            id[i] = st[i]+48;
                        }
                        id[5]='\0';
                    sprintf(temp.to_id,"cir%s",id);
                    c = create_circles(mysql,&temp);
                    temp.status[2]='h';
                    printf("shuchu\n");
                    b = insert_circledata(mysql,&temp);
                    if(c&&b)
                    {
                        char sprin[200]={0};
                    sprintf(sprin,"创建群%s成功",temp.to_id);
                    logs(mysql,&temp,sprin);
                    printf("Created success!\n");
                    temp.back = 1;
                }
                else
                {
                    char sprin[200]={0};
                sprintf(sprin,"创建群%s失败",temp.to_id);
                logs(mysql,&temp,sprin);
                temp.back = -1;
                printf("Created failed\n");
            }
            write(conn_fd,&temp,sizeof(struct chat));
                break;    
            }
            case 8: //加群处理
            {
                int a,b;
                temp.command = add_circle;
                a = check_addcircle(mysql,&temp);
                b = check_personcircle(mysql,&temp);
                printf("\t\t\t\ta = %d b = %d\n",a,b);
                if(a&&b)
                {
                    char sprin[200]={0};
                    sprintf(sprin,"加入群%s成功",temp.to_id);
                    logs(mysql,&temp,sprin);
                    temp.status[2]='m';
                    insert_circledata(mysql,&temp);
                    temp.back = 1;
                }
                else
                {
                    char sprin[200]={0};
                    sprintf(sprin,"加入群%s失败",temp.to_id);
                    logs(mysql,&temp,sprin);
                    temp.back = -1;
                }
                write(conn_fd,&temp,sizeof(struct chat));
                break;       
            }
            case 9: //退群
            {
                int a,b;
                temp.command = out_circle;
                a = check_addcircle(mysql,&temp);
                b = check_personcircle(mysql,&temp);
                if((a==1)&&(b==0))
                {
                    a = outto_circle(mysql,&temp);
                    if(a==1)
                    {
                        char sprin[200]={0};
                        sprintf(sprin,"退出群%s成功",temp.to_id);
                        logs(mysql,&temp,sprin);
                        temp.back = 1;
                    }
                    else{
                    char sprin[200]={0};
                    sprintf(sprin,"退出群%s失败",temp.to_id);
                    logs(mysql,&temp,sprin);
                        temp.back = -1;
                    }
                }
                else{
                    char sprin[200]={0};
                    sprintf(sprin,"输入群号码%s有问题",temp.to_id);
                    logs(mysql,&temp,sprin);
                    temp.back = -2;
                }
                write(conn_fd,&temp,sizeof(struct chat));
                break;
            }
            case 10: //解散该群
            {
                printf("\t\t\t\tjiesan qun\n");
                int a;
                temp.command = close_circle;
                a = host_circle(mysql,&temp);
                if(a==0)//没有该群
                {
                    char sprin[200]={0};
                    sprintf(sprin,"没有该群%s",temp.to_id);
                        logs(mysql,&temp,sprin);
                        temp.back = -1;
                    }
                    if(a==-1)//不是该群群主
                    {
                        char sprin[200]={0};
                        sprintf(sprin,"你不是群%s的群主",temp.to_id);
                        logs(mysql,&temp,sprin);
                        temp.back = -2;
                    }
                    if(a==1)//可以解散该群
                    {
                        int b;
                        b = close_circles(mysql,&temp);
                        if(b)
                        {
                        char sprin[200]={0};
                        sprintf(sprin,"解散群%s成功",temp.to_id);
                        logs(mysql,&temp,sprin);
                            temp.back = 1;
                        }
                        else{
                        char sprin[200]={0};
                        sprintf(sprin,"解散群%s失败",temp.to_id);
                        logs(mysql,&temp,sprin);
                            temp.back = -3;
                        }
                    }
                    write(conn_fd,&temp,sizeof(struct chat));
                    break;

                }
                case 11: //给客户端发送验证消息
                {
                    char *sprin="查看验证消息";
                    logs(mysql,&temp,sprin);
                    printf("mingling11\n");
                    temp.command = check_imformation;
                    get_imformation(mysql,&temp,conn_fd);
                    printf("jieshu\n");
                   break; 
                }
                case 12: //给客户端发送好友离线消息
                {
                    char *sprin="查看离线消息";
                    logs(mysql,&temp,sprin);
                    printf("lixian\n");
                    temp.command = friend_imformation;
                    get_imformation(mysql,&temp,conn_fd);
                    printf("lixian\n");
                    break;
                }
                case 13:  //给客户端发送系统的消息
                {
                    char *sprin="查看系统消息";
                    logs(mysql,&temp,sprin);
                    printf("xitong\n");
                    temp.command = system_imformation;
                    get_imformation(mysql,&temp,conn_fd);
                    printf("xitong\n");
                    break;
                }
                case 14: //处理回复好友消息
                {
                    char *sprin="处理验证消息";
                    logs(mysql,&temp,sprin);
                    printf("回复好友");
                    printf("好友:%s,%s",temp.to_id,temp.user_id);
                    //printf("标记:%c",temp.status[2]);
                    if(temp.status[2] == '1')
                    {
                        printf("\n\t\t>>>%s\n",temp.user_name);
                        printf("\n\t\t>>>%s\n",temp.user_id);
                        save_onlinefriend(mysql,&temp);
                       save_linefriend(mysql,&temp);
                        //save_friend(mysql,&temp);
                        insert_intofriend(mysql,&temp);
                        printf(">>><<<<<\n");
                        /*操作好友列表，加入*/
                    }
                    if(temp.status[2] == '0')
                    {
                        printf("该好友不同意添加");
                        /*回复对方不同意*/
                    }
                    break;
                }
                case 15: //查看所在的群
                {
                    char *sprin="查看所在的群";
                    logs(mysql,&temp,sprin);
                    int a;
                    temp.command = look_circle;
                    a = look_circles(mysql,&temp,conn_fd);
                    break;
                }
                case 16: //进入群聊
                {
                    char sprin[200]={0};
                    sprintf(sprin,"进入%s的群聊",temp.to_id);
                    logs(mysql,&temp,sprin);
                    int a,b;
                    char masg[200]={0};
                    strcpy(masg,temp.masg);
                    temp.command = all_chat;
                    a = check_addcircle(mysql,&temp);
                    b = check_personcircle(mysql,&temp);
                    memset(temp.masg,0,200);
                    strcpy(temp.masg,masg);
                    if((a==1)&&(b==0))
                    {
                        allchat(mysql,&temp,conn_fd,phead);
                    }
                    break;
                }
                case 17: //查看群聊天记录
                {
                    char sprin[200]={0};
                    sprintf(sprin,"查看%s的群聊天记录",temp.to_id);
                    logs(mysql,&temp,sprin);
                    look_allchat_history(mysql,&temp,conn_fd);
                    break;
                }
                case 18: //发送文件处理
                {
                    char sprin[200]={0};
                    sprintf(sprin,"给%s发送文件",temp.to_id);
                    logs(mysql,&temp,sprin);
                    char to_id[16]={0};
                    int coon;
                    strcpy(to_id,temp.to_id);
                    printf("\n\t\t\t\tto_id:%s\n",to_id);
                    coon = prin(phead,to_id);
                    if(conn_fd==-1)
                    {
                        temp.command = send_files;
                        temp.back = -1;//该ID不在线
                    }
                        else
                        {
                            temp.command = send_files;
                            if(temp.lens==225)
                            {
                                temp.back = 2;
                            }
                            if(temp.lens < 225)
                            {
                                temp.back = 1;
                            }
                            //temp.back = 1;
                            write(coon,&temp,sizeof(struct chat));
                        }
                        printf("\t\t\t\t\tsend file!\n\t\t\t");
                        printf("\n\t\t\t\tfilename:%s\n",temp.masg);
                        printf("\n\t\t\t\tdatas:%s\n",temp.datas);
                        printf("\n\t\t\t\tlength:%d\n",temp.lens);

                        break;
                    }
                }
        }
    }
}
int main(int argc,char **argv)
{
    timer = time(NULL);
    tblock = localtime(&timer);
    phead = (struct connect *)malloc(sizeof(struct connect));
    r = phead; 
    int sock_fd,conn_fd;//服务器套接字，客户端套接字
    struct sockaddr_in cli_add,ser_add;//服务器，客户端的地址结构
    char re_buf[32];
    int opt = 1;   /* 套接字选项 */
    pid_t pid;//创建线程
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    memset(&ser_add,0,sizeof(struct sockaddr));
    ser_add.sin_family=AF_INET;
    ser_add.sin_port=4507;
    ser_add.sin_addr.s_addr = htonl(INADDR_ANY);
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//设置套接字属性
    bind(sock_fd,(struct sockaddr *)&ser_add,sizeof(struct sockaddr_in));//绑定服务器套接字
    listen(sock_fd,LISTENQ);//服务器 监听
    socklen_t len = sizeof(struct sockaddr_in);//客户端的地址结构的长度
    r = phead;
    mysql = my_sql_init(mysql);
    mysql = mysql_connect(mysql);
    while(1)
    {
        printf("waite\n");
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
