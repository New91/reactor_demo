#include<stdio.h>
#include <strings.h>
#include <string.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <time.h>


int main(int argc, char **argv)
{

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

    int ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret == -1){
        fprintf(stderr, "bind socket error");
        return -1;
    }

    // 3. 开始监听
    ret = listen(serv_sock, 10);
     if(ret == -1){
        fprintf(stderr, "listen socket error");
        return -1;
    }

    struct sockaddr_in clt_addr;
    char buff[1024] = {0};//缓冲区
    time_t ticks;
    int conn_fd  = 0;
    while(1)
    {
        int len = sizeof(clt_addr);
        conn_fd = accept(serv_sock, (struct sockaddr *)&clt_addr, (socklen_t *)&len);
        printf("Info: connect from %s, port %d\n", 
            inet_ntop(AF_INET, &clt_addr.sin_addr, buff, sizeof(buff)),
            ntohs(clt_addr.sin_port));
        
        ticks = time(NULL);
        snprintf(buff,sizeof(buff),"%.24s\r\n", ctime(&ticks));
         
       ret = write(conn_fd, buff, strlen(buff));
        if(ret == -1){
            fprintf(stderr, "write socket error");
            return -1;
        }

        close(conn_fd);
    }

    close(conn_fd);
    printf("hello world!\n");
    return 0;
}
