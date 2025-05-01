#include"head.h"
int initTcp(char *serverip,char*prot)
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(prot));
    serverAddr.sin_addr.s_addr=inet_addr(serverip);
    int reues=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reues,sizeof(reues));
    bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(struct sockaddr_in));
    listen(sockfd,50);
    return sockfd;
}


