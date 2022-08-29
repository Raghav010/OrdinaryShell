#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "./err/errors.h"
#include "./shPrompt/prompt.h"
#include "./print_wd/print_wd.h"

int homedirLen=-1;



int main()
{
    //used to validate error throws
    int ret;
    char* sret;

    //setting color scheme for this shell(purple)
    printf("\033[1;35m");


    char input[1000];
    while(strcmp(input,"exit")!=0)
    {
        shPrompt();
        scanf("%s",input);
        if(strcmp(input,"pwd")==0)
            wd();
        //should a prompt be printed for just \n too??
    }
    return 0;
}

