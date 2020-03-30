#include "common.h"

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
    
    time_t ticks;
    int conn_fd  = 0;
    pid_t child_pid = 0;
    while(1)
    {
        int len = sizeof(clt_addr);
        conn_fd = accept(serv_sock, (struct sockaddr *)&clt_addr, (socklen_t *)&len);
        
        if ((child_pid = fork()) == 0)// 子进程来处理连接的业务逻辑
        {
            close(serv_sock);
            do_it(conn_fd);
            exit(0);
        }

        close(conn_fd);
    }

    printf("hello world!\n");
    return 0;
}
