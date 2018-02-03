/*************************************************************************
	> File Name: ces.c
	> Author: hujialu
	> Mail: 
	> Created Time: 2017年08月25日 星期五 13时28分18秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int input(char *st)
{
    char *p = st;
    int a=0;
   /* do{
        p++;
        *p = getchar();
       // p++;
    }while(*p!='\n');*/
    scanf("%[^\n]",st);
    //fgets(st,200,stdin);
    printf("st:%s\n",st);
}
int main()
{
    char *st;
    char *sr;
    sr = (char *)malloc(200);
    st = (char *)malloc(200);
   // gets(st);
    scanf("%s",sr);
    setbuf(stdin,NULL);
    //printf("\n");
    printf("inout:");
    //scanf("%[^\n]",st);
    input(st);
    printf("st:%s\n",st);
}
