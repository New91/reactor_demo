#include "common.h"

int max(int a, int b){
    return a>b?a:b;
}

static void do_it(FILE *fp, int sockfd)
{
    int read_cnt = 0;
    int maxfdp1;
    fd_set rset;
    char sendline[MAX_BUFF_LEN];
    char recvline[MAX_BUFF_LEN] = {0};
    int stdineof = 0;
    
    for(;;)
    {
        FD_ZERO(&rset);
        if(stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        select(maxfdp1, &rset, NULL, NULL, NULL);//可以同时监控输入流和套接字

        //网络套接字是否就绪
        if(FD_ISSET(sockfd, &rset)){
             if((read_cnt = read(sockfd, recvline, sizeof(recvline))) == 0){
                 if(stdineof == 1)
                    return;
                else{
                    fprintf(stderr, "readline error\n");
                    exit(0);
                }
             }
             recvline[read_cnt] = 0;
            fputs(recvline, stdout);
        }

        //输入流是否就绪
        if(FD_ISSET(fileno(fp), &rset)){
            if(fgets(sendline, MAX_BUFF_LEN, fp) == NULL){
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);//send FIN
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            write(sockfd, sendline, strlen(sendline));
        }
    }
}
int main(int argc, char **argv)
{
    if(argc != 2){
        fprintf(stderr, "usage: client <IP>\n");
        return 0;
    }

    int i,sockfd[5];
    for(int i = 0; i<5;i++)
    {
        sockfd[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sockfd[i] == -1){
            fprintf(stderr, "create socket error");
            return -1;
        }

        struct sockaddr_in svr_addr;
        bzero(&svr_addr, sizeof(svr_addr));
        svr_addr.sin_family = AF_INET;
        svr_addr.sin_port = htons(1024);
        inet_pton(AF_INET, argv[1], &svr_addr.sin_addr);
        connect(sockfd[i], (struct sockaddr *)&svr_addr, sizeof(svr_addr));
    }
     
    do_it(stdin, sockfd[0]);

    return 0;
}