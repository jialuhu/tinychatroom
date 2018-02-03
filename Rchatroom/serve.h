/*************************************************************************
	> File Name: serve.h
	> Author: hujialu
	> Mail: 
	> Created Time: 2017年08月12日 星期六 09时18分13秒
 ************************************************************************/
MYSQL *insert_data(MYSQL *mysql,struct chat *temp);
MYSQL *my_sql_init(MYSQL *mysql);
MYSQL *mysql_connect(MYSQL *mysql);
MYSQL *create_table(MYSQL *mysql,char *table_name);
int login_find(MYSQL *mysql,struct chat *temp);
char *U_ID(char *ch);
int online_flag(MYSQL *mysql,struct chat *temp);
int change(MYSQL *mysql,struct chat *temp,int choice);
int apply_friend(MYSQL *mysql,char *msg,struct chat *temp);
int find_msg(MYSQL *mysql,struct chat *temp,int con_fd);
//int add_friend(MYSQL *mysql,int con_fd,struct chat *temp);
int dele_cmsg(MYSQL *mysql,int con_fd,struct chat *temp);
int bianli_friends(MYSQL *mysql,struct chat *temp,int con_fd);
int bianli_user(MYSQL *mysql,char *id,int con_fd);
MYSQL find_online(MYSQL *mysql,struct chat *temp);//查看好友是否在线
MYSQL find_reg(MYSQL *mysql,struct chat *temp); //判断是否有该好友
MYSQL insert_imformation(MYSQL *mysql,struct chat *temp);
MYSQL get_imformation(MYSQL *mysql,struct chat *temp,int con_fd);
int insert_intofriend(MYSQL *mysql,struct chat *temp);
int check_refriend(MYSQL *mysql,struct chat *temp);
int prin_onlinefriend(MYSQL *mysql,struct chat *temp,int con_fd);
int prin_linefriend(MYSQL *mysql,struct chat *temp,int con_fd,int flag);
int save_friend(MYSQL *mysql,struct chat *temp);
int save_linefriend(MYSQL *mysql,struct chat *temp);
int save_onlinefriend(MYSQL *mysql,struct chat *temp);
int all_friend(MYSQL *mysql,struct chat *temp,int con_fd);
int all_myfriend(MYSQL *mysql,struct chat *temp,int con_fd,int flag);
int save_friendchat(MYSQL *mysql,struct chat *temp);
int save_mychat(MYSQL *mysql,struct chat *temp);
void chat_history(MYSQL *mysql,struct chat *temp,int con_fd);
int create_circles(MYSQL *mysql,struct chat *temp);
int insert_circledata(MYSQL *mysql,struct chat *temp);
int check_addcircle(MYSQL *mysql,struct chat *temp);
int check_personcircle(MYSQL *mysql,struct chat *temp);
int outto_circle(MYSQL *mysql,struct chat *temp);
int host_circle(MYSQL *mysql,struct chat *temp);
int close_circles(MYSQL *mysql,struct chat *temp);
int look_circles(MYSQL *mysql,struct chat *temp,int con_fd);
int allchat(MYSQL *mysql,struct chat *temp,int con_fd,struct connect *phead);
int send_allchat(MYSQL *mysql,struct chat *temp);
int look_allchat_history(MYSQL *mysql,struct chat *temp,int con_fd);
int delete_friends(MYSQL *mysql,struct chat *temp);
int deletefriends(MYSQL *mysql,struct chat *temp);
int logs(MYSQL *mysql,struct chat *temp,char *sprin);
