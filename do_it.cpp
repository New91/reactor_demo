#include "common.h"

void do_it(int sockfd)
{
    ssize_t n;
    char buff[MAX_BUFF_LEN] = {0};//缓冲区

again:
    while((n= read(sockfd, buff, MAX_BUFF_LEN)) > 0)
    {
        write(sockfd, buff, n);//读到什么就返回什么
        if(n<0 && errno == EINTR){
            goto again;
        }else if (n<0){
            fprintf(stderr, "write socket error");
        }
    }
}