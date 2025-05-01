#include<kk.h>
typedef struct train_s
{
    int length;
    char data[1000];
}train_t;

//train send&recv
int sendt(int netfd,char *str);
int recvt(int netfd,char *str);

//进入系统界面界面
int enter(int netfd);
int login(int netfd);
int registerm(int netfd);
int logoff(int netfd);

int putsfile(int netfd,char *name);
int getsfile(int netfd,char *name);
