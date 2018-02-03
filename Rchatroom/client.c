/*************************************************************************
	> File Name: ceshi1.c
	> Author: 
	> Mail: 
	> Created Time: 三  8/ 9 20:51:55 2017
 ************************************************************************/
 
#include"data.h"
#include<pthread.h>
//#include"create_sock.h"
#define HOST "localhost"
#define USER "root"
#define PASSWD " "
#define DB_NAME "chatroom"

void  m_write(int sock_fd,struct chat *temp);
struct chat *reg(struct chat *pstchat,int sock_fd);   //用户注册函数
void my_err(const char *err_string,int line);
void m_connect(int sock_fd,struct sockaddr_in *ser);
int create_sock();
void  m_write(int sock_fd,struct chat *temp);
void leg_insert();//注册界面
void login_inset();//登陆界面
int juge_login(int conn_fd,struct chat *temp);//客户端发送id和密码给服务端
void maininsert();//主界面菜单
int user_log(struct chat *temp,int conn_fd);//客户端发送id和密码给服务端*/
void addfriend(int con_fd,struct chat *temp);//加好友
void look_information(int con_fd,struct chat *temp);
void user_insert();
struct m *add_masg(struct m *phead,struct chat *temp);//链表增加验证消息
void personchat(struct chat *temp,int con_fd);
void circle_manage(int con_fd,struct chat *temp);//群管理
void circle_chat(int con_fd,struct chat *temp);
int send_file(int con_fd,struct chat *temp);

char y[3]="yes";
char quit[4]="quit";

struct m{
    char masg[200];
    char id[16];
    struct m *next;
};

struct m *phead;
struct m *r;
struct m *p;
time_t timer;
struct tm *tblock;

void my_err(const char *err_string,int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_string);
    exit(1);
}
int create_sock()
{
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
        my_err("socket",__LINE__);
    }
    else
    {
        return sock_fd;
    }
}
struct m *delete_masg(struct m *phead,char *id)//链表删除已经处理过的消息
{
    struct m *p = phead;
    struct m *pre = phead;
    if(p==NULL)
    {
        return phead;
    }
    while(p != NULL)
    {
        if(!strcmp(p->id,id))
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

/*int prin(struct m *phead,char *id)//查找在线节点conn_fd;
{
    int a;
    struct m *p = phead->next;
    while(p!=NULL)
    {
        if(!strcmp(p->id,id))
        {
            a = p->connect_id;  
            return a;
            break;
        }
        p = p->next;
    }
    return -1;
}*/

struct m *add_masg(struct m *phead,struct chat *temp)//链表增加验证消息
{
    struct m *p=phead;
    struct m *r,*new;
    new = (struct m *)malloc(sizeof(struct m));
    strcpy(new->masg,temp->masg);//记录每次登陆连接的套接字
    strcpy(new->id,temp->to_id);//为之后的用户Id做准备
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
void m_connect(int sock_fd,struct sockaddr_in *ser)
{
    if(connect(sock_fd, (struct sockaddr *)ser, sizeof(struct sockaddr)) < 0)
    {
        my_err("connect",__LINE__);
        exit(1);
    }
}

void  m_write(int sock_fd,struct chat *temp)
{
    int n;
    n = write(sock_fd,temp, sizeof(struct chat));
    if(n<0)
    {
        my_err("write",__LINE__);
    }
}

void leg_insert()//注册界面
{
    printf("\n\t\t\t\t┌──────────────────────────────────────────────────────────────────┐\n");
    printf("\t\t\t\t│--------------------------欢迎来到注册页面-------------------------│\n");
    printf("\t\t\t\t│───────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-------------------------------------------------------------------│\n");
    printf("\t\t\t\t│-----------------------请按照文字提示录入信息！--------------------│\n");
    printf("\t\t\t\t│------------*****注意：昵称为字母，在20个字符之间*****-------------│\n");
    printf("\t\t\t\t│------------***********密码为字母，密码为最多16位*****-------------│\n");
    printf("\t\t\t\t└───────────────────────────────────────────────────────────────────┘\n");
    printf("\n\n\n\n\n");
}

void login_inset()//登陆界面
{
   // system("echo '\033[;36m\n\t\t\t\t▂ ▃ ▄ ▅ ▆ ▇ █欢迎来到登陆页面！█ ▇ ▆ ▅ ▄ ▃ ▂ \033[0m'|pv -qL 5\t\t\t\t");
    printf("\n\n");
    printf("\n\t\t\t\t┌─────────────────────────────────────────────────────────────────┐\n");
    printf("\t\t\t\t│--------------------------欢迎来到登陆页面-------------------------│\n");
    printf("\t\t\t\t│───────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-------------------------------------------------------------------│\n");
    printf("\t\t\t\t│-----------------------请按照文字提示录入信息！--------------------│\n");
    printf("\t\t\t\t│------------*****注意：昵称为字母，在20个字符之间*****-------------│\n");
    printf("\t\t\t\t│------------***********密码为字母，密码为最多16位*****-------------│\n");
    printf("\t\t\t\t└───────────────────────────────────────────────────────────────────┘\n");
    printf("\n\n\n\n\n");
}
void maininsert()//主界面菜单
{
    printf("\n\t\t\t\t┌───────────────────────────────────────────────────────────────────┐\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-------------************欢迎来到聊天室************-------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------1、注册-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------2、登陆-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------3、帮助-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------**********请按照文字提示录入信息！***********---------│\n");
    printf("\t\t\t\t│----------------------------------------------------------------│\n");
    printf("\t\t\t\t└────────────────────────────────────────────────────────────────┘\n");
    printf("\n\t\t\t\t请输入阿拉伯数字进行选择：\n");
}
void friend()
{
    system("clear -l");
    printf("\n\t\t\t\t┌───────────────────────────────────────────────────────────────┐\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-------------************欢迎来到好友管理************-----------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------1、加好友---------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------2、删好友---------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------3、在线好友-------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------4、离线好友-------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------5、返回上级目录---------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------**********请按照文字提示录入信息！***********---------│\n");
    printf("\t\t\t\t│----------------------------------------------------------------│\n");
    printf("\t\t\t\t└────────────────────────────────────────────────────────────────┘\n");
    
}

void user_insert()
{
    printf("\n\t\t\t\t┌───────────────────────────────────────────────────────────────┐\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-------------************欢迎来到聊天室************-------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------1、好友管理-------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------2、私聊-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------3、群聊-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------4、群管理---------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------5、消息盒子-------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------6、退出-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------**********请按照文字提示录入信息！***********---------│\n");
    printf("\t\t\t\t│----------------------------------------------------------------│\n");
    printf("\t\t\t\t└────────────────────────────────────────────────────────────────┘\n");
    
}

void circle_manage(int con_fd,struct chat *temp)
{
    system("clear -l");
    printf("\n\t\t\t\t┌───────────────────────────────────────────────────────────────┐\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-------------************欢迎来到群管理************-------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------1、建群-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------2、加群-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------3、退群-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------4、解散群---------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------5、返回-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------**********请按照文字提示录入信息！***********---------│\n");
    printf("\t\t\t\t│----------------------------------------------------------------│\n");
    printf("\t\t\t\t└────────────────────────────────────────────────────────────────┘\n");
    int choise;
    printf("\n\t\t\t\t请输入要选择的选择:\n\t\t\t\t");
    scanf("%d",&choise);
    switch(choise)
    {
        case 1: ///创建群
        {
            system("clear -l");
            setbuf(stdin,NULL);
            temp->command = 7;
            printf("\n\t\t\t\t请输入要创建的群名称:\n\t\t\t\t");
            scanf("%s",temp->masg);
            write(con_fd,temp,sizeof(struct chat));
            sleep(2);
            printf("\n\t\t\t\t是否返回上一层界面,返回输入1不返回输入0:\n\t\t\t\t");
            int c;
            scanf("%d",&c);
            if(c==1)
            {
                return circle_manage(con_fd,temp);
            }
            break;
        }
        case 2://加群
        {
            system("clear -l");
            setbuf(stdin,NULL);
            temp->command = 8;
            printf("\n\t\t\t\t请输入要加入的群的群号:\n\t\t\t\t");
            scanf("%s",temp->to_id);
            write(con_fd,temp,sizeof(struct chat));
            sleep(2);
            int c;
            printf("\n\t\t\t\t是否返回上层目录,返回输入1不返回输入0:\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                return circle_manage(con_fd,temp);
            }
            break;
        }
        case 3://退群
        {
            system("clear -l");
            setbuf(stdin,NULL);
            temp->command = 15;
            write(con_fd,temp,sizeof(struct chat));
            printf("\n\t\t\t\t以下为您所在的群:\n\t\t\t\t");
            sleep(3);
            temp->command = 9;
            //sleep(3);
            printf("\n\t\t\t\t请输入要退出的群的群号:\n\t\t\t\t");
            scanf("%s",temp->to_id);
            write(con_fd,temp,sizeof(struct chat));
            sleep(2);
            int c;
            printf("\n\t\t\t\t是否返回上层目录,返回输入1不返回输入0:\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                return circle_manage(con_fd,temp);
            }
            break;
        }
        case 4: //解散群
        {
            system("clear -l");
            setbuf(stdin,NULL);
            temp->command = 15;
            write(con_fd,temp,sizeof(struct chat));
            printf("\n\t\t\t\t以下为您所在的群:\n\t\t\t\t");
            sleep(3);
            temp->command = 10;
            printf("\n\t\t\t\t请输入你要解散的群:\n\t\t\t\t");
            scanf("%s",temp->to_id);
            write(con_fd,temp,sizeof(struct chat));
            sleep(2);
            int c;
            printf("是否返回上一层目录,返回输入1不返回输入0:\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                return circle_manage(con_fd,temp);
            }
            break;
        }
        case 5:
        {
            return;
        }
    }

    
}

void circle_chat(int con_fd,struct chat *temp)
{
    system("clear -l");
    printf("\n\t\t\t\t┌───────────────────────────────────────────────────────────────┐\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-------------***********欢迎来到群聊界面***********-------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│--------------------------1、查看群-----------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│--------------------------2、选择群聊---------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│--------------------------3、聊天记录---------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│--------------------------4、返回-------------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------**********请按照文字提示录入信息！***********---------│\n");
    printf("\t\t\t\t│----------------------------------------------------------------│\n");
    printf("\t\t\t\t└────────────────────────────────────────────────────────────────┘\n");
    int choise;
    printf("\n\t\t\t\t请输入要选择的选项:\n\t\t\t\t");
    scanf("%d",&choise);
    switch(choise)
    {
        case 1://查看所在群
        {
            int c;
            temp->command = 15;
            write(con_fd,temp,sizeof(struct chat));
            //sleep(3);
            printf("\n\t\t\t\t以下为您所在的群:\n\t\t\t\t");
            sleep(3);
            printf("\n\t\t\t\t是否返回上层目录,返回输入1,不返回输入0:\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                return circle_chat(con_fd,temp);
            }
            break;
        }
        case 2://选择群聊
        {
            int end = 1;
            char End[4]="end";
            temp->command = 15;
            write(con_fd,temp,sizeof(struct chat));
            printf("\n\t\t\t\t以下为您所在的群:\n\t\t\t\t");
            memset(temp->masg,0,200);
            sleep(3);
            printf("\n\t\t\t\t请输入要群聊的群号:\n\t\t\t\t");
            scanf("%s",temp->to_id);
            temp->command = 16;
            system("clear -l");
            printf("\n\n\n\n\n\n");
            printf("请输入要输入的内容,输入end 结束:\n");
            while(end)
            {
                setbuf(stdin,NULL);
                scanf("%[^\n]",temp->masg);
                if(strcmp(temp->masg,End)==0)
                {
                    printf("退出此次群聊,请稍后\n");
                    sleep(3);
                    end = 0;
                    return circle_chat(con_fd,temp);
                }
                else{
                    write(con_fd,temp,sizeof(struct chat));
                }
            }
            break;
        }
        case 3://获取群聊聊天记录
        {
            int c;
            temp->command = 15;
            write(con_fd,temp,sizeof(struct chat));
            printf("\n\t\t\t\t以下为您所在的群:\n\t\t\t\t");
            memset(temp->masg,0,200);
            temp->command = 17;
            sleep(1);
            printf("\n\t\t\t\t请输入要查看哪个群的聊天记录:\n\t\t\t\t");
            scanf("%s",temp->to_id);
            write(con_fd,temp,sizeof(struct chat));
            printf("\n\t\t\t\t该群聊天记录如下:\n\t\t\t\t");
            sleep(4);
            printf("\n\t\t\t\t是否要返回，返回输入1,不返回输入0:\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                return circle_chat(con_fd,temp);
            }
            break;
        }
        case 4://返回
        {
            return;
        }
    }

}

void look_information(int con_fd,struct chat *temp)
{
    system("clear -l");
    printf("\n\n");
    printf("\t\t\t\t│──────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-----------------------1、验证消息------------------------│\n");
    printf("\t\t\t\t│──────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-----------------------2、好友消息------------------------│\n");
    printf("\t\t\t\t│──────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-----------------------3、系统消息------------------------│\n");
    printf("\t\t\t\t│──────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-----------------------4、处理验证消息--------------------│\n");
    printf("\t\t\t\t│──────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-----------------------5、返回上级目录--------------------│\n");
    printf("\t\t\t\t│──────────────────────────────────────────────────────────│\n");
    int choise;
    printf("\n\t\t\t\t请输入选项:\n\t\t\t\t");
    scanf("%d",&choise);
    switch(choise)
    {
        case 1:   //向服务器获取验证消息
        {
            int c;
            system("clear -l");
            temp->command = 11;
            temp->back = 0;
            temp->status[1]='c';
            write(con_fd,temp,sizeof(struct chat));
            printf("\n\t\t\t\t");
            printf("\n\t\t\t\t是否返回上层目录(0/1)\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                temp->command = 0;
                temp->back = 0;
                return look_information(con_fd,temp);
            }
            break;
        }
        case 2:  //向服务器获取好友离线消息
        {
            system("clear -l");
            int c;
            temp->command = 12;
            temp->back = 0;
            temp->status[1]='f';
            write(con_fd,temp,sizeof(struct chat));
            printf("\n\t\t\t\t是否返回上层目录(0/1)\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                temp->command = 0;
                temp->back = 0;
                return look_information(con_fd,temp);
            }
            break;
        }
        case 3:  //向服务器获取系统消息
        {
            system("clear -l");
            int c;
            temp->command = 13;
            temp->back = 0;
            temp->status[1]='s';
            write(con_fd,temp,sizeof(struct chat));
            printf("\n\t\t\t\t是否返回上层目录(0/1)\n\t\t\t\t");
            printf("\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                temp->command = 0;
                temp->back = 0;
                return look_information(con_fd,temp);
            }
            break;
        }
        case 4:
        {
            system("clear -l");
            temp->command = 14;
            int c;
            struct m *q=phead->next;
            struct m *r = q;
            char yes[4]="yes";
            char no[3]="no";
            if(q==NULL)
            {
                printf("\n\t\t\t\t暂时没有可以要处理的验证消息或者先到验证消息获取最近验证消息\n\t\t\t\t");
            }
        
            while(q!=NULL)
            {
               // int len=write(con_fd,temp,sizeof(struct chat));
                //printf("len=%d\n",len);
                r = q->next;
                char reply[4]={0};
                printf("\n\t\t\t\t账号:%s\n",q->id);
                printf("\n\t\t\t\t请求:%s\n",q->masg);
                printf("\n\t\t\t\t请回复yes或者no\n\t\t\t\t");
                printf("\t\t\t\t");
                scanf("%s",reply);
                if(strcmp(yes,reply)==0)
                {
                    strcpy(temp->to_id,q->id);
                   // temp->command = 14;
                    temp->status[2] = '1';
                }
                if(strcmp(no,reply)==0)
                {
                    strcpy(temp->to_id,q->id);
                    //temp->command = 14;
                    temp->status[2] = '0';
                }
                printf("temp.name:%s",temp->user_name);
                printf("temp.id:%s",temp->user_id);
                printf("temp.name:%s",temp->to_id);
                printf("temp.name:%s",temp->masg);
                printf("\n\n\n");
                int lne=write(con_fd,temp,sizeof(struct chat));
                printf("lne:%d\n",lne);
                printf("temp.command:%d\n",temp->command);
                free(q);
                //phead = r;
                q = r;
            }
            printf("\n\t\t\t\t是否返回上层目录(0/1)\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                temp->command = 0;
                temp->back = 0;
                phead -> next = NULL;/////////////////////////////
                return look_information(con_fd,temp);
            }
            break;
        }
        case 5:
        {
            return;
        }
    }
}
void friendmanage(int con_fd,struct chat *temp)//好友管理
{
    friend();
    int choise;
    printf("\n\t\t\t\t请选择输入选项数字:\n\t\t\t\t");
    scanf("%d",&choise);
    switch(choise)
    {
        case 1: //加好友
        {
            int c;
            addfriend(con_fd,temp);
            sleep(1);
            printf("\n\t\t\t\t是否返回上一层界面(0/1)\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                temp->command=0;
                temp->back = 0;
                memset(temp->masg,0,sizeof(1000));
                return friendmanage(con_fd,temp);
            }
            break;
        }
        case 2: //删除好友
        {
            temp->command = 6;
            temp->status[2]='0';
            write(con_fd,temp,sizeof(struct chat));
            int c;
            sleep(1);
            temp->command = 2;
            printf("\n\t\t\t\t请输入要删除好友的账号:\n\t\t\t\t");
            scanf("%s",temp->to_id);
            write(con_fd,temp,sizeof(struct chat));
            break;
        }
        case 3: //查看在线好友
        {
            int c;
            temp->command = 4;
            temp->status[2]='1';
            int a = write(con_fd,temp,sizeof(struct chat));
            sleep(4);
            printf("\n\t\t\t\t是否返回上一层界面(0/1)\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                temp->command=0;
                temp->back = 0;
                memset(temp->masg,0,sizeof(200));
                return;
            }
            break;
        }
        case 4: //查看离线好友
        {
            int c;
            temp->command = 5;
            temp->status[2]='0';
            //printf("<<<<<temp.command:%d",temp->command);
            int a = write(con_fd,temp,sizeof(struct chat));
            sleep(1);
            printf("\n\t\t\t\t是否返回上一层界面(0/1)\n\t\t\t\t");
            scanf("%d",&c);
            if(c==1)
            {
                temp->command=0;
                temp->back = 0;
                memset(temp->masg,0,sizeof(200));
                return;
            }
            break;
        }
        case 5:
        {
            return;
        }
    }
}

void addfriend(int con_fd,struct chat *temp)//加好友
{
    sprintf(temp->masg,"%s apply to become your friend,please reply.",temp->user_name);
    temp->command = 1;
    printf("\n\t\t\t\t请输入要添加的好友的账号:\n\t\t\t\t");
    scanf("%s",temp->to_id);
    int a = write(con_fd,temp,sizeof(struct chat));
    printf("a=%d\n",a);
    printf("\n\t\t\t\t已经发送，结果请查看系统消息\n\t\t\t\t");
}

int user_login(struct chat *temp,int conn_fd)//客户端发送id和密码给服务端
{
    char id[16]={0};
    char passwd[16]={0};
    int result=0;
    system("clear -l");
    login_inset();//登陆界面
    printf("\n\t\t\t\t请输入账号:\n\t\t\t\t");
    scanf("%s",temp->user_id);
    id[strlen(id)+1]='\0';
    printf("\n\t\t\t\t请输入密码:\n\t\t\t\t");
    scanf("%s",temp->user_passwd);
    temp->command=2;
    temp->back=-1;
    int a = write(conn_fd,temp,sizeof(struct chat));
    if(a>0)
    {
        int len = read(conn_fd,temp,sizeof(struct chat));
        if(temp->back==1)
        {
            printf("temp->back:%d\n",temp->back);
            printf("\n\t\t\t\t登陆成功,请稍后......\n\t\t\t\t");
            sleep(4);
            printf("\n");
            return 0;
        }
        if(temp->back==0)
        {
            printf("temp->back:%d\n",temp->back);
            printf("\n\t\t\t\t该账号已经登陆,4秒后即将返回登陆界面\n\t\t\t\t");
            sleep(4);
            return user_login(temp,conn_fd);
        }
        if(temp->back<0)
        {
            printf("\n\t\t\t\t账号或者密码错误,4秒后将返回登陆界面\n\t\t\t\t");
            sleep(4);
            return user_login(temp,conn_fd);
        }
    }
}

int user_log(struct chat *temp,int con_fd)  //注册函数
{
    system("clear -l");
    printf("\n\n\n\n\n\n\n");
    temp->command=1;
    char pass[16];
    int end = 1;
    while(end){
        printf("\n\t\t\t\t请输入用户名称:\n\t\t\t\t");
        scanf("%s",temp->user_name);
        printf("\n\t\t\t\t请输入密码:\n\t\t\t\t");
        scanf("%s",pass);
        printf("\n\t\t\t\t请在输入一次密码:\n\t\t\t\t");
        scanf("%s",temp->user_passwd);
        if(strcmp(pass,temp->user_passwd)==0)
        {
            m_write(con_fd,temp);
            printf("\n\t\t\t\t正在注册中.....\n\t\t\t\t");
            sleep(4);
            //memset(temp,0,sizeof(struct chat));
            read(con_fd,temp,sizeof(struct chat));
            if(temp->back==1)
            {
                printf("\n\t\t\t\t注册成功\n\t\t\t\t");
                printf("\n\t\t\t\t请记住您的登陆账号:%s\n\t\t\t\t",temp->user_id);
            }
            if(temp->back==-1)
            {
                printf("\n\t\t\t\t注册不成功\n\t\t\t\t");
            }
            end = 0;
            break;
        }
        else
        {
            printf("\n\t\t\t\t两次密码不匹配，返回重新注册\n\t\t\t\t");
        }
    }
}

void *cmd(void *arg)
{
    int fd = *((int *)arg);
    struct chat temp;
    int n;
    while(1)
    {
        n = read(fd,&temp,sizeof(struct chat));
        if(n==0)
        {
            printf("\n\t\t\t\t服务器不提供服务\n\t\t\t\t");
            break;
        }
        switch(temp.command)
        {
            case add_friend:  //添加好友
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\t有用户给你发来好友请求\n\t\t\t\t");
                    sleep(3);
                }
                if(temp.back == -2)
                {
                    printf("\n\t\t\t\t该用户已经下线，已经发送离线消息\n\t\t\t\t");
                }
                if(temp.back == -1)
                {
                    printf("\n\t\t\t\t该账号还未被注册\n\t\t\t\t");
                    printf("\n\t\t\t\t请重新输入\n\t\t\t\t");
                }
                if(temp.back == -3)
                {
                    printf("\n\t\t\t\t该用户已经是你的好友了\n\t\t\t\t");
                }
                if(temp.back==-4)
                {
                    printf("\n\t\t\t\t不能自己加自己\n\t\t\t\t");
                }
                break;
            }
            case delete_friend:  //删除好友
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\t删除成功\n\t\t\t\t");
                }
                break;
            }
            case look_online:  //查看在线好友
            {
                sleep(2);
                if(temp.back == -1)
                {
                    printf("\n\t\t\t\t暂时没有在线好友\n\t\t\t\t");
                }
                else{
                printf("\n\t\t\t\t在线好友：\n");
                printf("\n\t\t\t\t%s %s %s\n",temp.user_name,temp.to_id,temp.masg);
                    sleep(1);
                     }
                break;
            }
            case look_outline:  //查看不在线好友
            {
                if(temp.back == -1)
                {
                    printf("\n\t\t\t\t暂时没有离线好友\n\t\t\t\t");
                }
                else
                {
                    printf("\n\t\t\t\t离线好友：\n");
                    printf("\n\t\t\t\t%s %s %s\n",temp.user_name,temp.to_id,temp.masg);
                    sleep(1);
                }
                break;
            }
            case person_chat:   //私聊
            {
                if(temp.back == 1)
                {
                    //sleep(1);
                    printf("\n\t\t\t Name:%s\t ID:%s\n\t\t\t\t",temp.user_name,temp.to_id);
                }
                if(temp.back == -1)
                {
                    printf("\n\t\t\t\t该ID号不是你的好友或者该ID暂时未注册，请输入end结束\n\t\t\t\t");
                }
                if(temp.back == -2)
                {
                    printf("\n该好友不在线已经发送离线消息\n");
                }
                if(temp.back==-3)
                {
                    printf("\n\t\t\t\t暂时没有好友列表\n\t\t\t\t");
                }
                if(temp.back == 3)
                {
                    printf("\nname:%s\tmasg:%stime:%s\n",temp.user_name,temp.masg,asctime(tblock));
                   // setbuf(stdin,NULL);
                }
                if(temp.back==4)
                {
                    printf("\n%s:%s\n",temp.user_name,temp.masg);
                }
                if(temp.back == -4)
                {
                    printf("\n\t\t\t\t暂时没有聊天记录\n\t\t\t\t");
                }
                break;
            }
            case create_circle:  //创建群
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\t创建成功,群号为:%s\n\t\t\t\t",temp.to_id);

                }
                if(temp.back == -1)
                {
                    printf("\n\t\t\t\t创建失败,请稍后再尝试\n\t\t\t\t");
                }
                break;
            }
            case all_chat:     //群聊
            { 
                if(temp.back==1)
                {
                    printf("\n\t\t\t\tname:%s\tcircle_masg:%s\n",temp.user_name,temp.masg);
                }
                if(temp.back==-1)
                {
                    printf("\n\t\t\t\t已经发送离线消息,暂时没有一个好友在线\n");
                }
                break;
            }
            case look_circle:   //查看已加群
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\tcircle_name:%-20s\tcircle_id:%-20s\n\t\t\t\t",temp.user_name,temp.to_id);
                }
                if(temp.back==-1)
                {
                    printf("\n\t\t\t\t暂时没有加入任何群聊\n\t\t\t\t");
                }
                break;
            }
            case add_circle:    //添加群
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\t添加群成功\n\t\t\t\t");
                }
                if(temp.back==-1)
                {
                    printf("\n\t\t\t\t加群失败,请重新尝试\n\t\t\t\t");
                }
                break;
            }
            case out_circle:   //退出群
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\t你已经退群成功\n\t\t\t\t");
                }
                if(temp.back == -1)
                {
                    printf("\n\t\t\t\t退出失败,请稍后再尝试\n\t\t\t\t");
                }
                if(temp.back==-2)
                {
                    printf("\n\t\t\t\t输入的群号有问题,请核对再输入\n\t\t\t\t");
                }
                break;
            }
            case close_circle: //解散群
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\t解散成功\n\t\t\t\t");
                }
                if(temp.back==-1)
                {
                    printf("\n\t\t\t\t该群号暂未注册\n\t\t\t\t");
                }
                if(temp.back==-2)
                {
                    printf("\n\t\t\t\t您暂时不是该群群主,不能解散该群\n\t\t\t\t");
                }
                break;
            }
            case look_circle_history://查看群消息记录
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\t%s said: %s.\n",temp.user_name,temp.masg);
                }
                if(temp.back==-1)
                {
                    printf("\n\t\t\t\t暂时没有群消息\n\t\t\t\t");
                }
                break;
                
            }
            case check_imformation:  //查看好友验证消息
            {
                if(temp.back==1)
                {
                    phead = add_masg(phead,&temp);
                    printf("\n\t\t\t\t有验证消息\n\t\t\t\t");
                    printf("\n\t\t\t\t%s\n\t\t\t\t",temp.masg);
                    printf("\n\t\t\t\t%s %s %s %s",temp.user_name,temp.user_id,temp.to_id,temp.masg);
                }
                if(temp.back == -1)
                {
                    printf("\n\t\t\t\t暂时没有验证消息可以读\n\t\t\t\t");
                }
                break;
            }
            case friend_imformation:  //查看好友离线消息
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\t好友离线消息\n\t\t\t\t");
                    printf("\n\t\t\t\tcome from: %s\t\tmasg:%s\n\t\t\t\t",temp.user_name,temp.masg);
                }
                if(temp.back==-1)
                {
                    printf("\n\t\t\t\t您暂时没有好友离线消息\n\t\t\t\t");
                }
                break;
            }
            case system_imformation: //查看系统消息
            {
                if(temp.back==1)
                {
                    printf("\n\t\t\t\t系统提示消息\n\t\t\t\t");
                    printf("\n\t\t\t\t%s\n\t\t\t\t",temp.masg);
                    sleep(1);
                }
                if(temp.back==-1)
                {
                    printf("\n\t\t\t\t您暂时没有系统消息\n\t\t\t\t");
                }
                break;
            }
            case send_files://接受文件传输
            {
                if(temp.back == -1)
                {
                    printf("\n\t\t\t\t该账号未在线或者不是您的好友,请另行选择\n");
                }
                if(temp.back == 1)
                {
                    printf("\n\t\t\t\t你有新文件,来自%s账号为:%s\n\t\t\t\t",temp.user_name,temp.user_id);
                    int fd;
                    int len;
                    char name[16]={0};
                    sprintf(name,"/home/jialu/file/%s",temp.masg);
                    if((fd = open(name, O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR))<0)
                    {
                        my_err("open:",__LINE__);
                    }
                    len = write(fd,temp.datas,temp.lens);
                    printf("\n\t\t\t\t请在file目录下查看\n\t\t\t\t");
                }
                if(temp.back == 2)
                {
                    int fd;
                    int len;
                    char name[16]={0};
                    sprintf(name,"/home/jialu/file/%s",temp.masg);
                    if((fd = open(name, O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR))<0)
                    {
                        my_err("open:",__LINE__);
                    }
                    len = write(fd,temp.datas,temp.lens);
                }
                break;        
            }
        }
    }

    memset(&temp, 0, sizeof(struct chat));
}

void personchat(struct chat *temp,int con_fd)//进入私聊界面
{
    system("clear -l");
    temp->command = 6;
    printf("\n\t\t\t\t:\n\t\t\t\t");
    printf("\n\t\t\t\t┌───────────────────────────────────────────────────────────────────┐\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│-------------************欢迎来到私聊界面***********------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------1、查看好友-------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------2、选择私聊-------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------3、聊天记录-------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------4、发送文件-------------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------------------------5、返回上级目录---------------------│\n");
    printf("\t\t\t\t│────────────────────────────────────────────────────────────────│\n");
    printf("\t\t\t\t│----------**********请按照文字提示录入信息！***********---------│\n");
    printf("\t\t\t\t│----------------------------------------------------------------│\n");
    printf("\t\t\t\t└────────────────────────────────────────────────────────────────┘\n");
    printf("\n\t\t\t\t请输入阿拉伯数字进行选择：\n\t\t\t\t");
    int choise;
    scanf("%d",&choise);
    switch(choise)
    {
        case 1://查看好友
        {
            temp->status[2]='0';
            write(con_fd,temp,sizeof(struct chat));
            break;
        }
        case 2://选择私聊
        {
            printf("\n\t\t\t\t以下是你的好友:\n\t\t\t\t");
            temp->status[2]='0';
            write(con_fd,temp,sizeof(struct chat));
            sleep(1);
            char End[4]="end";
            int end = 1;
            printf("\n\t\t\t\t请你输入要私聊的账号:\n");
            printf("\t\t\t\t");
            scanf("%s",temp->to_id);
            printf("\n");
            printf("\t\t\t\t正在进入请稍后.....\n\t\t\t\t");
            sleep(2);
            system("clear -l");
            printf("\n\n\n\n\n");
            printf("\n请输入您要私聊的内容\n");
            while(end)
            {
                setbuf(stdin,NULL);//清空键盘缓冲区
                char *st;
                st = (char *)malloc(300);
                temp->status[2]='1';
                sleep(2);
                scanf("%[^\n]",temp->masg);
                if(strcmp(temp->masg,End)==0)
                {
                    end = 0;
                    printf("\n\t\t\t\t感谢进入私聊，此次私聊结束\n\t\t\t\t");
                }
                else{
                    write(con_fd,temp,sizeof(struct chat));
                }
            }
            break;
        }
        case 3:  //查看聊天记录
        {
            temp->status[2]='2';
            printf("\n\t\t\t\t请输入要查看聊天记录的账号:\n\t\t\t\t");
            scanf("%s",temp->to_id);
            write(con_fd,temp,sizeof(struct chat));
            break;
        }
        case 4:  //发送文件
        {
            temp->status[2]='0';
            write(con_fd,temp,sizeof(struct chat));
            temp->command = 18;
            send_file(con_fd,temp);
            break;
        }
        case 5:
        {
            return;
        }
    }
    int c;
    sleep(3);
    printf("\n\t\t\t\t是否返回,不返回将退出此次登陆:\n\t\t\t\t");
    scanf("%d",&c);
    if(c==1)
    {
        return personchat(temp,con_fd);
    }
    else
    {
        exit(1);
    }
}

int send_file(int con_fd,struct chat *temp)
{
    int fd;
    int len;
    char file_name[16];
    int flag = 0;
    char datas[225]={0};
    printf("\n\t\t\t\t请输入发送对象的账号:\n\t\t\t\t");
    scanf("%s",temp->to_id);
    printf("\n\t\t\t\t请你输入要发送的文件的绝对路径:\n\t\t\t\t");
    scanf("%s",file_name);
    file_name[strlen(file_name)+1]='\0';
    printf("file_name:%s\n",file_name);
    if((fd=(open(file_name,O_RDONLY))) == -1)
    {
        my_err("open file:",__LINE__);
    }
    else
    {
        /*计算文件的字节数目*/
        if((lseek(fd, 0, SEEK_END)) == -1){
            my_err("lseek",__LINE__);
        }
        if((len=lseek(fd,0,SEEK_CUR)) == -1)
        {
            my_err("lseek",__LINE__);
        }
        if((lseek(fd, 0, SEEK_SET))==-1)//将文件指针置于开始
        {
            my_err("lseek",__LINE__);
        }
        printf("\t\t\t\tlen:%d\n",len);
        /*文件名称解析*/
        char *q = file_name;
        while(*q!='\0')
        {
            q++;
            if(*q=='/')
            {
                flag = 1;
                break;
            }
        }
        if(flag == 1)
        {
            char *p = file_name+strlen(file_name)-1;
            while(*p!='/')
            {
                p--;
            }
            p++;
            printf("99999\n");
            strcpy(temp->masg,p);
            printf("99999\n");
        }
        else
        {
            strcpy(temp->masg,file_name);
        }
        int length=0;
        int sum = 0;
            while(sum != len)
            {
            memset(datas,0,225);
            length = read(fd, datas, 225);
            sum = sum + length;
            memcpy(temp->datas,datas,length);
            temp->command = 18;
            temp->lens = length;
            write(con_fd,temp,sizeof(struct chat));
            }
    printf("\n\t\t\t\t发送成功\n\t\t\t\t");
    }
    close(fd);
}


int main()
{
    phead = (struct m *)malloc(sizeof(struct m));
    r = phead;
    int i;
    stchat temp;
    timer = time(NULL);
    tblock = localtime(&timer);
    struct chat *now;
    //MYSQL *mysql = mysql_init(NULL);
    struct sockaddr_in serv_add;
    stchat my;
    int con_fd;
    /*本地地址结构*/
    memset(&serv_add, 0, sizeof(struct sockaddr_in));
    serv_add.sin_family = AF_INET;
    serv_add.sin_port=4507;
    serv_add.sin_addr.s_addr=htonl(INADDR_ANY);

    /*创建套接字并且连接*/
    con_fd = create_sock();
    m_connect(con_fd, &serv_add);
    
    int flag=1;//这册标志判断
    while(flag)
    {
        int choise=0;
        memset(&temp,0,sizeof(struct chat));
        system("clear -l");
        maininsert();
        printf("\n\t\t\t\t请输入要选择的选项：\n");
        printf("\n\t\t\t\t");
        scanf("%d",&choise);
        switch(choise)
        {
            case 1://注册函数
            {
                user_log(&temp,con_fd);
                int c;
                printf("\n\t\t\t\t请输入1继续，输入0退出\n\t\t\t\t");
                scanf("%d",&c);
                if(c==0)
                {
                    exit(0);
                }
                break;
            }
            case 2: //登陆函数
            {
                int a;
                a = user_login(&temp,con_fd);
                flag = 0;
                break;
            }
        case 3:
        {
            printf("马上就写帮助说明");//帮助函数说明
            break;
        }
        default:
        {
            printf("输入选项不符合要求，请看提示输入选择选项！\n");
            break;
        }
    }
    }
    pthread_t thid;
    pthread_create(&thid,NULL,cmd,(void *)(&con_fd));
    int choise;
    while(1)
    {
        system("clear -l");
        printf("\n\t\t\t\t欢迎%s来到聊天室\n\t\t\t\t",temp.user_name);
        user_insert();
        printf("\n\t\t\t\t");
        scanf("%d",&choise);
        switch(choise)
        {
            case 1: //好友管理
            {
               // printf("siliao\n");
                //friend();
                friendmanage(con_fd,&temp);
                break;
            }
            case 2: //私聊
            {
                personchat(&temp,con_fd);
                break;
            }
            case 3: //群聊
            {
                circle_chat(con_fd,&temp);
                break;
            }
            case 4: //群管理
            {
                circle_manage(con_fd,&temp);
                break;
            }
            case 5: //消息盒子
            {
                look_information(con_fd,&temp);
                break;
            }
            case 6: //退出登陆
            {
                exit(1);
                sleep(2);
            }
        }
    }

}
