#include <signal.h>
#include <stdio.h>
#include "c-polyfill.h"

bool askedToStop = false;

void catchTerminateSignal()
{
    askedToStop = true;
}

void catchFatalSignal(int signo)
{
    if (signo == 11)
    {
        fprintf(stderr, "SEGMENTATION FAULT\n");
    }
    else
    {
        fprintf(stderr, "FATAL SIGNAL: %d\n", signo);
    }
}

void initSignals()
{
    signal(SIGINT, catchTerminateSignal);
    signal(SIGTERM, catchTerminateSignal);
    signal(SIGABRT, catchTerminateSignal);
    signal(SIGILL, catchFatalSignal);
    signal(SIGSEGV, catchFatalSignal);
}

bool signalledToStop()
{
    return askedToStop;
}
