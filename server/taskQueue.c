#include"head.h"

void initQueue(taskQueue_t *queue)
{
    queue->pTail=NULL;
    queue->pHead=NULL;
    queue->queuesize=0;
}
void enQueue(taskQueue_t *queue,int data)
{
    node_t *pNew=(node_t *)calloc(1,sizeof(node_t));
    pNew->netfd=data;
    if(queue->pHead==NULL){
        queue->pTail=pNew;
        queue->pHead=pNew;
        queue->queuesize++;
    }
    else{
        queue->pTail->pNext=pNew;
        queue->pTail=pNew;
        queue->queuesize++;
    }
}
int popQueue(taskQueue_t *queue,int *getnetfd)
{
    if(queue->queuesize==0){
        return 0;
    }
        *getnetfd=queue->pHead->netfd;
        printf("getnetfd=%d\n",*getnetfd);
    if(queue->queuesize==1){
        queue->pTail=NULL;
        queue->pHead=NULL;
        queue->queuesize=0;
    }
    else{
        *getnetfd=queue->pHead->netfd;
        node_t *pCur=queue->pHead;
        queue->pHead=queue->pHead->pNext;
        queue->queuesize--;
    }
}
void visitQueue(taskQueue_t queue)
{
    node_t *pCur=queue.pHead;
    while(pCur!=NULL)
    {
        printf("%d ",pCur->netfd);
        pCur=pCur->pNext;
    }
    printf("\n");
}

