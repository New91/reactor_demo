#include "common.h"

static void do_it(FILE *fp, int sockfd)
{
    char sendline[MAX_BUFF_LEN] = {0};
    char recvline[MAX_BUFF_LEN] = {0};
    int read_cnt = 0;
    while(fgets(sendline, MAX_BUFF_LEN, fp) != NULL){
        write(sockfd, sendline, strlen(sendline));
        if((read_cnt = read(sockfd, recvline, sizeof(recvline))) < 0){
            fprintf(stderr, "readline error\n");
            exit(0);
        }

        fputs(recvline, stdout);
    }
}
int main(int argc, char **argv)
{
    if(argc != 2){
        fprintf(stderr, "usage: client <IP>\n");
        return 0;
    }
     int clt_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clt_sock == -1){
        fprintf(stderr, "create socket error");
        return -1;
    }

    struct sockaddr_in svr_addr;
    bzero(&svr_addr, sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(1024);
    inet_pton(AF_INET, argv[1], &svr_addr.sin_addr);
    connect(clt_sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr));

    do_it(stdin, clt_sock);

    return 0;
}