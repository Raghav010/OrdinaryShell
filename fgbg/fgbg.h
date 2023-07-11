#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 
#include "../gvars.h"
#include <string.h>
#include <errno.h>
#include "../err/errors.h"


struct bgps
{
    int bgpid;
    char bgcmd[20];
    int stopped; //whether the process is stopped
    int jobnumber;
};



//signal handler
void printTer(int signum);


//initilaize all bgpids
void bgpsInit();

//continues a stopped background process
int bgContinue();


int sendSignal();

int printJobs();


int bringToFg();

void fgbgHandler(char* command,int bg,int justWait);

void printfgMsg();

void printbgMsg();