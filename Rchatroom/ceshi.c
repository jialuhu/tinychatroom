/*************************************************************************
	> File Name: ceshi.c
	> Author: hujialu
	> Mail: 
	> Created Time: 2017年08月17日 星期四 08时44分05秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct chat{
    char name[16];
    char id[16];
    char to_id[16];
    int command;
    int back;
    char masg[10];
};

struct chat *string_ling()
{
    struct chat B;
    char *p=(char *)malloc(sizeof(struct chat));
    memset(&B,0,sizeof(struct chat));
    memcpy(p,&st,sizeof(struct chat));
    memcpy(&B,p,sizeof(struct chat));
    return ;
}

int main()
{
    char *name="titi";
    char *id="90345";
    char *masg="lkdsjflaj";
    struct chat T;
    char *p=(char *)malloc(sizeof(struct chat));
    struct chat *B;
    B = (struct chat *)malloc(sizeof(struct chat));
    memset(&T,0,sizeof(struct chat));
    strcpy(T.name,name);
    strcpy(T.id,id);
    strcpy(T.masg,masg);
    T.command=1;
   /* char *p;
    p = (char *)malloc(sizeof(struct chat));
    memcpy(p,&T,sizeof(struct chat));
    memcpy(&B,p,sizeof(struct chat));
    printf("%s %s %d\n",B.name,B.id,B.back);*/
    B = string_ling(T);
    printf("%s %s %d\n",B.name,B.id,B.back);

}
