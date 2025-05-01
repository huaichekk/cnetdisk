#include"head.h"
#include"md5.h"
void *pthreadFunc(void *arg)
{
    threadPoll_t *pthreadPoll=(threadPoll_t*)arg;
    int netfd;
    char username[1024]={0};
    while(1){
        pthread_mutex_lock(&pthreadPoll->mutex);
        while(pthreadPoll->taskQueue.queuesize==0){
            printf("tid:%ld wait to work\n",pthread_self());
            pthread_cond_wait(&pthreadPoll->cond,&pthreadPoll->mutex);
        }
        printf("i start to connect mysql\n");
    pthreadPoll->fmysql=mysql_init(NULL);
    MYSQL *cret=mysql_real_connect(pthreadPoll->fmysql,"localhost","root","3209","netdisk",0,NULL,0);
    if(cret==NULL){
        fprintf(stderr,"mysql_real_connect:%s\n",mysql_error(pthreadPoll->fmysql));
        return NULL;
    }

        printf("i connect mysql\n");
        strcpy(username,pthreadPoll->taskQueue.pHead->username);
        popQueue(&pthreadPoll->taskQueue,&netfd);
        printf("tid:%ld get a work | netfd=%d username=%s\n",pthread_self(),netfd,username);
        pthread_cond_broadcast(&pthreadPoll->cond);
        pthread_mutex_unlock(&pthreadPoll->mutex);
        evenLoop(netfd,pthreadPoll->fmysql,username);
    }
}
int makeWorker(int workerNum,workerData_t *workerArr,threadPoll_t *pthreadPoll)
{
    for(int i=0;i<workerNum;i++)
    {
        pthread_create(&workerArr[i].tid,NULL,pthreadFunc,pthreadPoll);
    }
    return 0;
}
int evenLoop(int netfd,MYSQL *mysql,char *username)
{   
    char sql[1024]={0};
    stackf_t stackf;
    initstack(&stackf);

    bzero(sql,sizeof(sql));
    sprintf(sql,"select id from file where user='%s' and pre_id=-1;",username);
    mysql_query(mysql,sql);
    MYSQL_RES *res=mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(res);
    int id=atoi(row[0]);
    enstack(&stackf,id);
    printf("stack top=%d\n",stackf.id[stackf.topd]);
    while(1){
        char arg1[64]={0};
        char arg2[64]={0};
        recvt(netfd,arg1);
        if((strcmp(arg1,"ls")==0)||strcmp(arg1,"pwd")==0){
        }
        else{
            recvt(netfd,arg2);
        }
        if(strcmp(arg1,"mkdir")==0){
            //得到父id和path
            int fid=stacktop(&stackf);
            bzero(sql,sizeof(sql));
            sprintf(sql,"select count(*) from file where pre_id=%d and filename='%s';",fid,arg2);
            mysql_query(mysql,sql);
            MYSQL_RES *cres=mysql_store_result(mysql);
            MYSQL_ROW crow=mysql_fetch_row(cres);
            int cnt=atoi(crow[0]);
            if(cnt!=0){
                sendt(netfd,"name repeat!");
                continue;
            }

            char fpath[1024]={0};
            char mpath[1024]={0};
            bzero(sql,sizeof(sql));
            sprintf(sql,"select path from file where id=%d;",fid);
            mysql_query(mysql,sql);
            MYSQL_RES *res=mysql_store_result(mysql);
            MYSQL_ROW row=mysql_fetch_row(res);
            strcpy(fpath,row[0]);
            sprintf(mpath,"%s%s/",fpath,arg2);
            bzero(sql,sizeof(sql));
            sprintf(sql,"insert into file(filename,user,pre_id,path,type) values('%s','%s','%d','%s','DIR');",arg2,username,fid,mpath);
            mysql_query(mysql,sql);
        }
        if(strcmp("ls",arg1)==0){
            char str[1024]={0};
            int fid=stacktop(&stackf);


            bzero(sql,sizeof(sql));
            sprintf(sql,"select filename from file where pre_id=%d;",fid);
            mysql_query(mysql,sql);
            MYSQL_RES *res=mysql_store_result(mysql);
            int rnum=mysql_num_rows(res);
            if(rnum==0){
                printf("empty set\n");
                continue;
            }
            for(int i=0;i<mysql_num_rows(res);i++){
                MYSQL_ROW row=mysql_fetch_row(res);
                sprintf(str,"%s  %s",str,row[0]);
            }
               sendt(netfd,str);
        }
        if(strcmp("pwd",arg1)==0){
            int fid=stacktop(&stackf);
            bzero(sql,sizeof(sql));
            sprintf(sql,"select path from file where id=%d;",fid);
            mysql_query(mysql,sql);
            MYSQL_RES *res=mysql_store_result(mysql);
            MYSQL_ROW row=mysql_fetch_row(res);
            sendt(netfd,row[0]);
        }
        if(strcmp("rmdir",arg1)==0){
            int fid=stacktop(&stackf);
            bzero(sql,sizeof(sql));
            sprintf(sql,"delete from file where pre_id=%d and filename='%s';",fid,arg2);
            mysql_query(mysql,sql);
        }
        if(strcmp("cd",arg1)==0){
            int fid=stacktop(&stackf);
            char *p=strtok(arg2,"/");
                printf("p1=%s\n",p);
            if(strcmp(p,"..")==0){
                popstack(&stackf);
            }
            else if(strcmp(p,".")!=0){
                bzero(sql,sizeof(sql));
                sprintf(sql,"select id from file where pre_id=%d and filename='%s';",fid,p);
                mysql_query(mysql,sql);
                MYSQL_RES *res=mysql_store_result(mysql);
                int rnum=mysql_num_rows(res);
                if(rnum==0){
                    sendt(netfd,"Don`t find such directry");
                    continue;
                }
                MYSQL_ROW row=mysql_fetch_row(res);
                int newfid=atoi(row[0]);
                enstack(&stackf,newfid);
            }


            while(1){
                p=strtok(NULL,"/");
                if(p==NULL){
                    break;
                }
                printf("p=%s\n",p);
                if(strcmp(p,"..")==0){
                    popstack(&stackf);
                }
                else if(strcmp(p,".")!=0){
                    fid=stacktop(&stackf);
                    printf("fid=%d\n",fid);
                    bzero(sql,sizeof(sql));
                    sprintf(sql,"select id from file where pre_id=%d and filename='%s';",fid,p);
                    mysql_query(mysql,sql);
                    MYSQL_RES *res=mysql_store_result(mysql);
                    int rnum=mysql_num_rows(res);
                    if(rnum==0){
                        sendt(netfd,"Don`t find such directry");
                        break;
                    }
                    MYSQL_ROW row=mysql_fetch_row(res);
                    int newfid=atoi(row[0]);
                    enstack(&stackf,newfid);
                }
            }
        }
        if(strcmp("puts",arg1)==0){
            int fid=stacktop(&stackf);
            char buf[1024]={0};
            char realpath[1024]={0};
            char realpathmd5[1024]={0};
            off_t filesize;
            recvt(netfd,(char *)&filesize);
            printf("filesize=%ld\n",filesize);
            

            sprintf(realpath,"/home/huaichekk/netdisk/%s",arg2);
            int fd=open(realpath,O_RDWR|O_CREAT|O_TRUNC,0666);
            ftruncate(fd,filesize);
            char *p=(char *)mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
            ERROR_CHECK(p,NULL,"mmap");
            recv(netfd,p,filesize,MSG_WAITALL);
            munmap(p,filesize);
            
            char md5_str[MD5_STR_LEN+1]={0};
            Compute_file_md5(realpath,md5_str);
            printf("%s\n",md5_str);

            char fpath[1024]={0};
            char mpath[1024]={0};
            bzero(sql,sizeof(sql));
            sprintf(sql,"select path from file where id=%d;",fid);
            mysql_query(mysql,sql);
            MYSQL_RES *res=mysql_store_result(mysql);
            MYSQL_ROW row=mysql_fetch_row(res);
            strcpy(fpath,row[0]);
            sprintf(mpath,"%s%s/",fpath,arg2);
            bzero(sql,sizeof(sql));
            sprintf(sql,"insert into file(filename,user,pre_id,path,type,md5) values('%s','%s','%d','%s','FILE','%s');",arg2,username,fid,mpath,md5_str);
            
            mysql_query(mysql,sql);
            sprintf(realpathmd5,"/home/huaichekk/netdisk/%s",md5_str);
            rename(realpath,realpathmd5);

            close(fd);

        }
        if(strcmp("rm",arg1)==0){
            int fid=stacktop(&stackf);
            bzero(sql,sizeof(sql));
            char realpath[1024]={0};
            sprintf(sql,"select md5 from file where pre_id=%d and filename='%s';",fid,arg2);
            mysql_query(mysql,sql);
            MYSQL_RES *res=mysql_store_result(mysql);
            int rnum=mysql_num_rows(res);
            if(rnum==0){
                sendt(netfd,"Don`t find such file");
                continue;
            }
            MYSQL_ROW row=mysql_fetch_row(res);
            sprintf(realpath,"/home/huaichekk/netdisk/%s",row[0]);
            unlink(realpath);
            bzero(sql,sizeof(sql));
            sprintf(sql,"delete from file where pre_id=%d and filename='%s';",fid,arg2);
            mysql_query(mysql,sql);

        }
        if(strcmp("gets",arg1)==0){
            int fid=stacktop(&stackf);
            bzero(sql,sizeof(sql));
            char realpath[1024]={0};
            sprintf(sql,"select md5 from file where pre_id=%d and filename='%s';",fid,arg2);
            mysql_query(mysql,sql);
            MYSQL_RES *res=mysql_store_result(mysql);
            int rnum=mysql_num_rows(res);
            if(rnum==0){
                sendt(netfd,"Don`t find such file");
                continue;
            }
            MYSQL_ROW row=mysql_fetch_row(res);
            sprintf(realpath,"/home/huaichekk/netdisk/%s",row[0]);
            int fd=open(realpath,O_RDWR);
            struct stat statbuf;
            fstat(fd,&statbuf);
            sendt(netfd,(char *)&statbuf.st_size);
            char *p=(char *)mmap(NULL,statbuf.st_size,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
            send(netfd,p,statbuf.st_size,MSG_NOSIGNAL);
            munmap(p,statbuf.st_size);
            close(fd);
        }

    }
}
