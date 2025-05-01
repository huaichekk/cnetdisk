#include"head.h"
int sendt(int netfd,char *str){
    train_t train;
    train.length=strlen(str);
    bzero(train.data,sizeof(train.data));
    memcpy(train.data,str,train.length);
    send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    return 0;
}
int recvt(int netfd,char *str){
    train_t train;
    recv(netfd,&train.length,sizeof(train.length),MSG_WAITALL);
    bzero(train.data,sizeof(train.data));
    recv(netfd,train.data,train.length,MSG_WAITALL);
    bzero(str,sizeof(str));
    memcpy(str,train.data,train.length);
    printf("recv train len=%d,data=%s\n",train.length,train.data);
    return 0;
}
