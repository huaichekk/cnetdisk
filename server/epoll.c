#include"head.h"
int epollAdd(int epfd,int netfd)
{
    struct epoll_event events;
    events.data.fd=netfd;
    events.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&events);
    return 0;
}
int epollDel(int epfd,int netfd)
{
    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd,NULL);
    return 0;
}
