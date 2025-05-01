#include"head.h"
int enter(int netfd){
    printf("---------------------\n");
    printf("Welcome to mynetdisk!\n");
    int ret;
    while(1){
        printf("1、login\n");
        printf("2、register\n");
        printf("3、log off\n");
        printf("0、exit\n");
        printf("please input your choice:\n");
        int choice;
        scanf("%d",&choice);
        switch(choice){
        case 1:
            ret=login(netfd);
            if(ret==0){
                return 0;
            }
            break;
        case 2:
            registerm(netfd);
            break;
        case 3:
            logoff(netfd);
            break;
        case 0:
            exit(0);
            break;
        default:
            printf("please input 0~3 !!!\n");
            break;
                
        }
    }
}
int login(int netfd){
    int choice=1;
    send(netfd,&choice,sizeof(choice),MSG_NOSIGNAL);
    //发送用户名和密码
    char username[30];
    char passwd[100];
    printf("Please input your username:\n");
    scanf("%s",username);
    sendt(netfd,username);
    printf("Please input you password\n");
    scanf("%s",passwd);
    sendt(netfd,passwd);
    
    int istrue;
    recv(netfd,&istrue,sizeof(istrue),MSG_WAITALL);
    if(istrue==-1){
        printf("username or password error or has a tomb!\n");
        return -1;
    }
    else{
        printf("Login success!\n");
        printf("---------------------------\n");
        return 0;
    }

    

}
int registerm(int netfd){
    int choice=2;
    send(netfd,&choice,sizeof(choice),MSG_NOSIGNAL);
    //发送用户名和密码
    char username[30];
    char passwd[100];
    printf("Please input your username:\n");
    scanf("%s",username);
    sendt(netfd,username);
    printf("Please input you password\n");
    scanf("%s",passwd);
    sendt(netfd,passwd);
    int istrue;
    recv(netfd,&istrue,sizeof(istrue),MSG_WAITALL);
    if(istrue==0){
        printf("register success!\n");
        return 0;
    }
    else{
        printf("repetitive username! register error!\n");
        return -1;
    }

}
int logoff(int netfd){
    int choice=3;
    send(netfd,&choice,sizeof(choice),MSG_NOSIGNAL);
    //发送用户名和密码
    char username[30];
    char passwd[100];
    printf("Please input your username:\n");
    scanf("%s",username);
    sendt(netfd,username);
    printf("Please input you password\n");
    scanf("%s",passwd);
    sendt(netfd,passwd);
    int istrue;
    recv(netfd,&istrue,sizeof(istrue),MSG_WAITALL);
    if(istrue==0){
        printf("logoff success!\n");
        return 0;
    }
    
}
