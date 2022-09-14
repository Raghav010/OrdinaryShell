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

//signal handler
void printTer(int signum);


//initilaize all bgpids
void bgpidsInit();


void fgbgHandler(char* command,int bg);

void printfgMsg();

void printbgMsg();