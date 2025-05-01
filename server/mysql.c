#include"head.h"
int initmysql(MYSQL *mysql){
    mysql=mysql_init(NULL);
    MYSQL *cret=mysql_real_connect(mysql,"localhost","root","3209","netdisk",0,NULL,0);
    if(cret==NULL){
        fprintf(stderr,"mysql_real_connect:%s\n",mysql_error(mysql));
        return -1;
    }
}
int checkuser(MYSQL *mysql,char *username){
    char sql[1024];
    sprintf(sql,"select count(*) from user where username='%s';",username);
    int qret=mysql_query(mysql,sql);
    if(qret!=0){
        fprintf(stderr,"mysql_query:%s\n",mysql_error(mysql));
        return -1;
    }
    MYSQL_RES *res=mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(res);
    int count=atoi(row[0]);
    if(count==0){
        return -1;
    }
    else if(count==1){
        return 0;
    }
}
int checkpasswd(MYSQL *mysql,char *username,char *passwd){
    char sql[1024]={0};
    sprintf(sql,"select passwd,salt from user where username='%s';",username);
    mysql_query(mysql,sql);
    MYSQL_RES *res=mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(res);
    char *p=crypt(passwd,row[1]);
    if(strcmp(p,row[0])==0){
        return 0;
    }
    else{
        return -1;
    }
}
int adduser(MYSQL *mysql,char *username,char *passwd){
    int ret=checkuser(mysql,username);
    if(ret==0){
        return -1;
    }
    char sql[1024]={0};
    char salt[21]={0};
    salt[20]='\0';
    salt[0]='$';
    salt[1]='6';
    salt[2]='$';
    salt[19]='$';
    int flag;
    srand(time(NULL));
    for(int i=3;i<19;i++){
        flag=rand()%3;
        switch(flag){
        case 0:
            salt[i]=rand()%26+'a';
            break;
        case 1:
            salt[i]=rand()%26+'A';
            break;
        case 2:
            salt[i]=rand()%10+'0';
            break;
        }
    }
    char *p=crypt(passwd,salt);
    sprintf(sql,"insert into user(username,salt,passwd,tomb) values('%s','%s','%s',0);",username,salt,p);
    mysql_query(mysql,sql);
    
    bzero(sql,sizeof(sql));
    sprintf(sql,"insert into file(filename,user,pre_id,path,type) values('/','%s',-1,'/','DIR');",username);
    mysql_query(mysql,sql);
    return 0;
}
int deluser(MYSQL *mysql,char *username,char *passwd){
    int ret=checkuser(mysql,username);
    if(ret==-1){
        return -1;
    }
    ret=checkpasswd(mysql,username,passwd);
    if(ret==-1){
        return -1;
    }
    char sql[1024]={0};
    sprintf(sql,"update user set tomb=1 where username='%s';",username);
    mysql_query(mysql,sql);
    return 0;
}
int islive(MYSQL *mysql,char *username){
    char sql[1024]={0};
    sprintf(sql,"select tomb from user where username='%s';",username);
    mysql_query(mysql,sql);
    MYSQL_RES *res=mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(res);
    int tomb=atoi(row[0]);
    if(tomb==1){
        return -1;
    }
    else{
        return 0;
    }
}
