#include "common.h"

#include <time.h>


int main(int argc, char **argv)
{
    int i,maxi,maxfd,listenfd,connfd,sockfd;
    int nready,client[FD_SETSIZE];
    ssize_t n;
    fd_set rset;
    fd_set allset;
    char buf[MAX_BUFF_LEN];
    socklen_t clilen;

    // 1. 服务器创建一个socket
    //  AF_INET             IPv4 Internet protocols          ip(7)
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serv_sock == -1){
        fprintf(stderr, "create socket error");
        return -1;
    }

    // 2. 绑定ip 端口
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(1024);
    // 2.1 设置端口重用
    int opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    int ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret == -1){
        fprintf(stderr, "bind socket error\n");
        return -1;
    }

    // 3. 开始监听
    ret = listen(serv_sock, 10);
     if(ret == -1){
        fprintf(stderr, "listen socket error\n");
        return -1;
    }

    struct sockaddr_in clt_addr;
    
    maxfd = serv_sock;
    maxi = -1;
    for(i = 0; i<FD_SETSIZE;i++)
        client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(serv_sock, &allset);

    while(1)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);

        if(FD_ISSET(serv_sock, &rset))//新的连接来了
        {
            clilen = sizeof(clt_addr);
            connfd = accept(serv_sock, (struct sockaddr *)&clt_addr, (socklen_t *)&clilen);

            for(i = 0; i<FD_SETSIZE;i++)
            {
                if(client[i]<0){
                    client[i] = connfd;
                    break;
                }
                if(i==FD_SETSIZE)
                    fprintf(stderr, "too many clients");
            }

            FD_SET(connfd, &allset);
            if(connfd > maxfd)
                maxfd = connfd;
            if(i>maxi)
                maxi = i;
            if(--nready <= 0)
                continue;
        }


        for(int i=0; i<=maxi; i++)
        {
            if((sockfd = client[i]) < 0)
                continue;
            if(FD_ISSET(sockfd, &rset)){
                if((n= read(sockfd, buf, MAX_BUFF_LEN)) == 0){
                    close(sockfd);
                    FD_CLR(sockfd,&allset);
                    client[i] = -1;
                }else{
                    write(sockfd, buf,n);
                }

                if(--nready <= 0)
                    break;
            }
        }
    }

    printf("hello world!\n");
    return 0;
}
