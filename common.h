#ifndef _COMMON_
#define _COMMON_

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define MAX_BUFF_LEN 1024

//定义一个新类型
//这个类型是一个函数
//是一个带有一个int参数，返回为void的函数
typedef void Sigfunc(int);

void do_it(int sockfd);
void sig_child(int signo);
Sigfunc *Signal(int signo, Sigfunc *func);

#endif // _COMMON_