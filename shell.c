#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./err/errors.h"
#include "./shPrompt/prompt.h"
#include "./print_wd/print_wd.h"
#include "./ech/ech.h"
#include "./change_dir/cdir.h"
#include "gvars.h"


char* input;


//maybe use fflush after commands that print something
//should commands return a success and failure code? instead of void
//add a short description about inputs and outputs of the function in the header files
//colorize commands and output
//add errors to handle invalid user input
//malloc all strings of length >= 1000

int main()
{
    //setting color scheme for this shell(purple)
    printf("\033[1;35m");

    //allocating buffer for input shared across all commands(inter file var)
    input=(char*)malloc(4000);
    if(input==NULL)
        return errorR(1); 

    
    char endtoken = '\n';  //used to signify when to print prompt
    while (1)
    {
        if (endtoken == '\n')
        {
            shPrompt();
        }
        //taking the first chunk of input(in case of ;)
        if (scanf("%[^;\n]", input) == 0)
        {
            scanf("%c", &endtoken);
            continue;
        }
        scanf("%c", &endtoken);  //scans 
        /*if(endtoken!='\n' && endtoken!=';')
        {
            printf("Input is too long\n");
            continue;
        }*/
        char *command = strtok(input, " \t");
        if (command == NULL)
            continue;
        else if (strcmp(command, "pwd") == 0)
            wd();
        else if (strcmp(command, "echo") == 0)
            echo();
        else if (strcmp(command, "cd") == 0)
            cd();
        else if (strcmp(command, "exit") == 0)
            break;
        else
            printf("Command not found\n");
    }
    free(input);
    return 0;
}

