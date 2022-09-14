#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "../gvars.h"
#include "../err/errors.h"
#include <string.h>


//parses input and calls discover
void parseInp();

int discover(char* parent,char* curdir,char* tofind,int* flags);