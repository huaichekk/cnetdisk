# mynetdisk

#### 介绍
多线程C/S服务端架构网盘


### 使用流程

1. 启动mysql服务器
2. ./server 192.168......  1234 3(参数分别为暴露的地址，端口，线程池的线程数)，启动server
3. 使用client连接服务端，并使用命令行对服务端进行操作

#### 使用示例
```
ls testDir
pwd
get testFile
put testFile
rm testFile
rmdir testDir
```
