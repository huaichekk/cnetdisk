#include"head.h"

int putsfile(int netfd,char *name){
    char buf[1024]={0};
    int fd=open(name,O_RDWR);
    struct stat statbuf;
    fstat(fd,&statbuf);
    sendt(netfd,(char *)&statbuf.st_size);

    char *p=(char *)mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(p,NULL,"mmap");
    send(netfd,p,statbuf.st_size,MSG_NOSIGNAL);
    munmap(p,statbuf.st_size);
    close(fd);

}
int getsfile(int netfd,char *name){
    
    off_t filesize;
    recvt(netfd,(char *)&filesize);
    int fd=open(name,O_RDWR|O_CREAT|O_TRUNC,0666);
    ftruncate(fd,filesize);

    char *p=(char *)mmap(NULL,filesize,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    ERROR_CHECK(p,NULL,"mmap");
    recv(netfd,p,filesize,MSG_WAITALL);
    munmap(p,filesize);
    close(fd);
}
