#include"head.h"
int main(int argc,char*argv[])
{
    // ./server 192--  1234 3
    ARGC_CHECK(argc,4);
    threadPoll_t threadPoll;
    int workerNum=atoi(argv[3]);
    workerData_t *workerArr=(workerData_t*)calloc(workerNum,sizeof(workerData_t));
    makeWorker(workerNum,workerArr,&threadPoll);
    int sockfd=initTcp(argv[1],argv[2]);

    initQueue(&threadPoll.taskQueue);
    pthread_mutex_init(&threadPoll.mutex,NULL);
    pthread_cond_init(&threadPoll.cond,NULL);
    
    MYSQL *user=mysql_init(NULL);
    MYSQL *cret=mysql_real_connect(user,"localhost","root","3209","netdisk",0,NULL,0);
    if(cret==NULL){
        fprintf(stderr,"mysql_real_connect:%s\n",mysql_error(user));
        return -1;
    }

    int epfd=epoll_create(1);
    printf("epfd=%d sockfd=%d \n",epfd,sockfd);
    epollAdd(epfd,sockfd);

    while(1)
    {
        struct epoll_event readySet[1024];
        int readyNum=epoll_wait(epfd,readySet,1024,-1);
        printf("1 client is connected\n");

        for(int i=0;i<readyNum;i++){
            if(readySet[i].data.fd==sockfd){
                int netfd=accept(sockfd,NULL,NULL);
                char username[1024]={0};
                enter(netfd,user,username);
                pthread_mutex_lock(&threadPoll.mutex);
                while(threadPoll.taskQueue.queuesize>=2)
                {
                    printf("i am host,i wait to send renwu!\n");
                    pthread_cond_wait(&threadPoll.cond,&threadPoll.mutex);
                }
                printf("i push 1 task to queue | netfd=%d\n",netfd);
                enQueue(&threadPoll.taskQueue,netfd);
                strcpy(threadPoll.taskQueue.pTail->username,username);
                pthread_cond_broadcast(&threadPoll.cond);
                pthread_mutex_unlock(&threadPoll.mutex);

            }
        }
    }

    



    return 0;
}

