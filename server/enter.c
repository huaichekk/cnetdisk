#include"head.h"
int enter(int netfd,MYSQL *mysql,char *pusername){
    while(1){

    int choice;
    int ret;
    recv(netfd,&choice,sizeof(choice),MSG_WAITALL);
    switch(choice){
    case 1:
        ret=login(netfd,mysql,pusername);
        if(ret==0){
            return 0;
        }
        break;
    case 2:
        registerm(netfd,mysql);
        break;
    case 3:
        logoff(netfd,mysql);
        break;
    default:
        break;
    }
    
    }
}
int login(int netfd,MYSQL *mysql,char *pusername){
    char username[30]={0};
    char passwd[100]={0};
    recvt(netfd,username);
    recvt(netfd,passwd); 
    printf("username=%s strlen=%ld\n",username,strlen(username));
    int ret=checkuser(mysql,username);
    if(ret==-1){
        printf("username false\n");
        int istrue=-1;
        send(netfd,&istrue,sizeof(istrue),MSG_NOSIGNAL);
        return -1;
    }
    ret=islive(mysql,username);
    if(ret==-1){
        printf("username repetion\n");
        int istrue=-1;
        send(netfd,&istrue,sizeof(istrue),MSG_NOSIGNAL);
        return -1;
    }
    ret=checkpasswd(mysql,username,passwd);
    if(ret==-1){
        printf("passwd false\n");
        int istrue=-1;
        send(netfd,&istrue,sizeof(istrue),MSG_NOSIGNAL);
        return -1;
    }
    printf("username and passwd true\n");
    strcpy(pusername,username);
    int istrue=0;
    send(netfd,&istrue,sizeof(istrue),MSG_NOSIGNAL);
    return 0;
}
int registerm(int netfd,MYSQL *mysql){
    char username[30]={0};
    char passwd[100]={0};
    recvt(netfd,username);
    recvt(netfd,passwd); 
    int ret=adduser(mysql,username,passwd);
    if(ret==-1){
        int istrue=-1;
        printf("register false\n");
        send(netfd,&istrue,sizeof(istrue),MSG_NOSIGNAL);
        return -1;
    }
    int istrue=0;
    send(netfd,&istrue,sizeof(istrue),MSG_NOSIGNAL);
    return 0;
}
int logoff(int netfd,MYSQL *mysql){
    char username[30]={0};
    char passwd[100]={0};
    recvt(netfd,username);
    recvt(netfd,passwd); 
    int ret=deluser(mysql,username,passwd);
    if(ret==-1){
        int istrue=-1;
        send(netfd,&istrue,sizeof(istrue),MSG_NOSIGNAL);
        return -1;
    }
    int istrue=0;
    send(netfd,&istrue,sizeof(istrue),MSG_NOSIGNAL);
    return 0;

}
