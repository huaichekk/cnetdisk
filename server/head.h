#include<kk.h>
typedef struct stackf_s{
    int id[1024];
    int topd;
}stackf_t;
typedef struct train_s{
    int length;
    char data[1024];
}train_t;
//工人结构体
typedef struct workerData_s{
   pthread_t tid;
}workerData_t;
//任务队列
typedef struct node_s{
    int netfd;
    char username[1024];
    struct node_s *pNext;
}node_t;
typedef struct taskQueue_s{
    node_t *pHead;
    node_t *pTail;
    int queuesize;
}taskQueue_t;
//进程池
typedef  struct threadPoll_s{
    MYSQL *fmysql;
    taskQueue_t taskQueue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}threadPoll_t;
//工人函数
int makeWorker(int workerNum,workerData_t *workerArr,threadPoll_t *pthreadPoll);
void *pthreadFunc(void *arg);
int evenLoop(int netfd,MYSQL *mysql,char *username);

//队列函数
void initQueue(taskQueue_t *queue);
void enQueue(taskQueue_t *queue,int data);
int popQueue(taskQueue_t *queue,int *getnetfd);
void visitQueue(taskQueue_t queue);
//epoll
int initTcp(char *serverip,char*prot);
int epollAdd(int epfd,int netfd);
int epollDel(int epfd,int netfd);
//train send&recv
int sendt(int netfd,char *str);
int recvt(int netfd,char *str);
//进入系统界面
int enter(int netfd,MYSQL *mysql,char *pusername);
int login(int netfd,MYSQL *mysql,char *pusername);
int registerm(int netfd,MYSQL *mysql);
int logoff(int netfd,MYSQL *mysql);

//mysql
int initmysql(MYSQL *mysql);
int checkuser(MYSQL *mysql,char *username);
int checkpasswd(MYSQL *mysql,char *username,char *passwd);
int adduser(MYSQL *mysql,char *username,char *passwd);
int deluser(MYSQL *mysql,char *username,char *passwd);
int islive(MYSQL *mysql,char *username);


//栈函数
int initstack(stackf_t *stackf);
int enstack(stackf_t *stackf,int id);
int popstack(stackf_t *stackf);
int stacktop(stackf_t *stackf);

