第一次写的low逼服务器，基于C/S模型
基本功能就是私聊、群聊、好友管理等等。是通过C语言操作数据库来实现的。数据库的表格设计的不太合理，每一个用户都建立了一张表去存储各种信息。因为每一个用户对应着不同的随机ID，所以ID成为了唯一的标示用户身份的东西。登陆等也采用ID账号来登陆。
由于自己的数据库有些问题，所以在编译和运行的时候有些不同。
编译的时候用的命令是：
(客户端 low服务器)
```
gcc client.c -o client -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient -lpthread

gcc serve1.c c-serve.c -o serve1 -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient -lpthread
```
其实可以编写一个make文件一键生成
在自己的环境下运行的时候，由于数据库的问题没有解决，所以在运行low服务器的时候需要sudo下运行，客户端则随意。。。

PS：自己可能要卸载自己的双系统，所以赶紧跑来把上个假期的小聊天室上传github，谜一般的突然上传。。。。
