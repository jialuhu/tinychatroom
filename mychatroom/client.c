/*************************************************************************
	> File Name: ceshi1.c
	> Author: 
	> Mail: 
	> Created Time: 三  8/ 9 20:51:55 2017
 ************************************************************************/
 
#include"data.h"
#define HOST "localhost"
#define USER "root"
#define PASSWD "97452"
#define DB_NAME "chatroom"
int main()
{
    int i;
    MYSQL *mysql = mysql_init(NULL);
    struct sockaddr_in serv_add;
    stchat my;
    int con_fd;
    if(!mysql)
    {
        printf("cuowu\n");
    }
    int b=0;
    if(!(mysql_real_connect(mysql,HOST,USER,PASSWD,DB_NAME,0,NULL,0)))
    {
        printf("b=%d\n",b);
    }
    memset(&serv_add, 0, sizeof(struct sockaddr_in));
    serv_add.sin_family = AF_INET;
    serv_add.sin_port=4507;
    serv_add.sin_addr.s_addr=htonl(INADDR_ANY);
    con_fd = socket(AF_INET,SOCK_STREAM, 0);
    if(connect(con_fd, (struct sockaddr *)&serv_add,sizeof(struct sockaddr))<0)
    {
        printf("connect is failed\n");
        }
    else
    {
        int choise=0;
        int flag=1;//这册标志判断
        //insert();//进入界面函数
        while(flag)
        {
            switch(choise)
            {
                case 0:
                {
                    memset(&my,0,sizeof(stchat));
                    int a;
                    //char r_buf[32]="hello";
                    strcpy(my.user_name,"jialu");
                    strcpy(my.user_id,"1234");
                    my.command=1;
                    a = write(con_fd,&my,sizeof(stchat));
                    //read(con_fd,w_buf)
                    printf("a=%d\n",a);
                    if(a>0)
                    {
                        flag =0;
                    }
                    //regist();//注册函数
                    break;
                }
            /*case LODIN:
            {
                int a;
                a = juge_regist();//判断是否注册
                if(a==0)
                {
                    flag=0;
                    break;
                }
                else{
                    int a;
                    a = LOGIN();//登录函数
                    if(a==0)
                    {
                        登录失败
                    }
                    else{
                        flag=0;//登录成功
                    }
                }
                break;
            }
            case HELP:
            {
                help();//帮助函数说明
            }*/
                case 1:
                {
                    printf("please intput the exit:\n");
                }
                default:
                {
                    printf("输入选项不符合要求，请看提示输入选择选项！\n");
                }
            }


        }
    }
    sleep(20);
}
