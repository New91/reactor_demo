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

#define MAX_BUFF_LEN 1024

void do_it(int sockfd);

#endif // _COMMON_