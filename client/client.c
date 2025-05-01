#include"head.h"
int main(int argc,char *argv[])
{
    // ./2_en.. 192.... 1234
    ARGC_CHECK(argc,3);
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr=inet_addr(argv[1]);
    int ret=connect(sockfd,(struct sockaddr*)&serverAddr,sizeof(struct sockaddr));
    ERROR_CHECK(ret,-1,"connect");
    printf("sockfd=%d connect ok\n",sockfd);
    enter(sockfd);

    int epfd=epoll_create(1);
    struct epoll_event events;
    events.events=EPOLLIN;
    events.data.fd=sockfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&events);
    events.events=EPOLLIN;
    events.data.fd=STDIN_FILENO;
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&events);
    
    char buf[1024]={0};
    while(1){
        struct epoll_event readySet[2];
        int readyNum=epoll_wait(epfd,readySet,2,-1);
        for(int i=0;i<readyNum;i++){
            if(readySet[i].data.fd==STDIN_FILENO){
                bzero(buf,sizeof(buf));
                read(STDIN_FILENO,buf,sizeof(buf));

                int cnt=1;
                char *q=strtok(buf,"\n");
                char *arg1=strtok(q," ");
                char arg2[64]={0};
                char *p;
                while(1){
                    p=strtok(NULL," ");
                    if(cnt==1&&p!=NULL){
                        strcpy(arg2,p);
                    }
                    if(p==NULL){
                        break;
                    }
                    cnt++;
                }
                if(cnt==1&&((strcmp("ls",arg1)==0)||(strcmp("pwd",arg1)==0))){
                    sendt(sockfd,arg1);
                    break;
                }
                if(cnt==2&&((strcmp("gets",arg1)==0)||(strcmp("puts",arg1)==0)||
                            (strcmp("mkdir",arg1)==0)||(strcmp("cd",arg1)==0)||
                            (strcmp("rm",arg1)==0)||(strcmp("rmdir",arg1)==0))){
                    sendt(sockfd,arg1);
                    sendt(sockfd,arg2);
                    if(strcmp("puts",arg1)==0){
                        putsfile(sockfd,arg2);
                    }
                    if(strcmp("gets",arg1)==0){
                        getsfile(sockfd,arg2);
                    }
                    break;
                }

                printf("command error!\n");
            }


            //读取sockfd
            if(readySet[i].data.fd==sockfd){
                recvt(sockfd,buf);
                printf("%s\n",buf);
            }
        }


    }
}
