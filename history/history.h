#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../err/errors.h"


char** createHq();


//returns command count
int loadH(char** q);

//do not pass a malloced string to this , increments command count
void addcmd(int* commandcount,char** q,char* cmd);

void saveH(char** q,int commandcount);

void printH(char** q,int commandcount);