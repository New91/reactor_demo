#include "common.h"


// from unp
Sigfunc *Signal(int signo, Sigfunc *func)
{
    struct sigaction act;
    struct sigaction oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if(sigaction(signo, &act, &oact) < 0)
    {
        return SIG_ERR;
    }

    return oact.sa_handler;
}

