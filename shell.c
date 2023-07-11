#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./err/errors.h"
#include "./shPrompt/prompt.h"
#include "./print_wd/print_wd.h"
#include "./ech/ech.h"
#include "./change_dir/cdir.h"
#include "./list_dir/ls.h"
#include "gvars.h"
#include "./history/history.h"
#include "./fgbg/fgbg.h"
#include "./pinfo/pinfo.h"
#include "./discover/discover.h"
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

char* input;


//maybe use fflush after commands that print something
//should commands return a success and failure code? instead of void
//add a short description about inputs and outputs of the function in the header files
//colorize commands and output
//add errors to handle invalid user input
//malloc all strings of length >= 1000
//create a wrapper for malloc
//add error handling in ls,history,fgbg




int main()
{
    //setting color scheme for this shell(purple)
    printf("\033[1;35m");


    //setting signal dispositions
    if(signal(SIGTSTP,SIG_IGN)==SIG_ERR)
        errorR(2);
    if(signal(SIGINT,SIG_IGN)==SIG_ERR)
        errorR(2);
    if(signal(SIGCLD,printTer)==SIG_ERR)
        errorR(2);

    //allocating buffer for input shared across all commands(inter file var)
    input=(char*)malloc(4096);
    if(input==NULL)
        return errorR(1); 

    char** hq=createHq();
    if(hq==NULL)
        return -1;
    int commandcount=loadH(hq);
    
    char endtoken = '\n';  //used to signify when to print prompt
    char entireline[512]; //make it bigger
    entireline[0]='\0';
    int prevPipeAv=0;

    bgpsInit();

    int pipeEnds[2]; //0 read end ; 1 write end

    while (1)
    {
        printbgMsg();
        if (endtoken == '\n')
        {
            shPrompt();
            printfgMsg();
        }
        //taking the first chunk of input(in case of ;)
        int inpStatus=scanf("%[^|&;\n]", input);
        if ( inpStatus == 0)
        {
            scanf("%c", &endtoken);
            if(endtoken=='\n')
            {
                addcmd(&commandcount,hq,entireline);
                entireline[0]='\0';
            }
            continue;
        }
        else if(inpStatus==-1) //EOF(^D) was entered
        {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        scanf("%c", &endtoken);  //scans 

        strncat(entireline,input,strlen(input));
        if(endtoken==';')
            strcat(entireline,";");
        if(endtoken=='&')
            strcat(entireline,"&");
        if(endtoken=='|')
            strcat(entireline,"|");
        if(endtoken=='\n')
        {
            addcmd(&commandcount,hq,entireline);
            entireline[0]='\0';
        }

        //save original file nummbers for stdout and stdin
        int origOUT,origIN;
        dup2(STDOUT_FILENO,origOUT);
        dup2(STDIN_FILENO,origIN);




        //piping---------------
        if(prevPipeAv)
        {
            close(pipeEnds[1]);
            dup2(pipeEnds[0],STDIN_FILENO);
        }

        if(endtoken=='|')
        {
            pipe(pipeEnds);
            dup2(pipeEnds[1],STDOUT_FILENO);
            prevPipeAv=1;
        }
        else
        {
            prevPipeAv=0;
        }



        //IO redirection--------------
        //checking for i/o redirection //does not handle tilda in files //close files!!!
        int k=0;
        int IORedirPos[3]={-1,-1,-1}; //<,>,>>
        

        //splitting string into different parts
        while(input[k]!='\0')
        {
            if(input[k]=='>')
            {
                input[k]='\0';
                if(input[k+1]!='\0' && input[k+1]=='>')
                {
                    IORedirPos[2]=k+2;
                    input[k+1]='\0';
                }
                else
                {
                    IORedirPos[1]=k+1;
                }
                break;
            }
            if(input[k]=='<')
            {
                IORedirPos[0]=k+1;
                input[k]='\0';
            }
            k++;
        }
        
        //handling input redirection, getting input file name and setting fds
        if(IORedirPos[0]!=-1)
        {
            int inFd;
            char* inFile=strtok(&input[IORedirPos[0]]," \t");
            inFd=open(inFile,O_RDONLY);
            if(inFd==-1)
            {
                errorR(1);
                dup2(origOUT, STDOUT_FILENO);
                dup2(origIN, STDIN_FILENO);
                continue;
            }
            dup2(inFd,STDIN_FILENO);
        }


        //handling output redirection, getting output file name and setting up fds
        int outFd;
        if(IORedirPos[2]!=-1)
        {
            char *outFile = strtok(&input[IORedirPos[2]], " \t");
            outFd = open(outFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(outFd==-1)
            {
                errorR(1);
                dup2(origOUT, STDOUT_FILENO);
                dup2(origIN, STDIN_FILENO);
                continue;
            }
            dup2(outFd,STDOUT_FILENO);
        }
        else if(IORedirPos[1]!=-1)
        {
            char *outFile = strtok(&input[IORedirPos[1]], " \t");
            outFd = open(outFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(outFd==-1)
            {
                errorR(1);
                dup2(origOUT, STDOUT_FILENO);
                dup2(origIN, STDIN_FILENO);
                continue;
            }
            dup2(outFd,STDOUT_FILENO);
        }

        

        /*if(endtoken!='\n' && endtoken!=';')
        {
            printf("Input is too long\n");
            continue;
        }*/
        char *command = strtok(input, " \t");
        if (command == NULL)
        {   
            dup2(origOUT,STDOUT_FILENO);
            dup2(origIN,STDIN_FILENO);
            continue;
        }
        else if (strcmp(command, "pwd") == 0)
            wd();
        else if (strcmp(command, "echo") == 0)
            echo();
        else if (strcmp(command, "cd") == 0)
            cd();
        else if(strcmp(command,"ls")==0)
            lsg();
        else if(strcmp(command,"history")==0)
            printH(hq,commandcount);
        else if(strcmp(command,"pinfo")==0)
            printPinfo();
        else if(strcmp(command,"discover")==0)
            parseInp();
        else if(strcmp(command,"jobs")==0)
            printJobs();
        else if(strcmp(command,"sig")==0)
            sendSignal();
        else if(strcmp(command,"bg")==0)
            bgContinue();
        else if(strcmp(command,"fg")==0)
            bringToFg();
        else if (strcmp(command, "exit") == 0)
            break;
        else
        {
            int bg=0;
            if(endtoken=='&')
            {
                bg=1;
            }
            fgbgHandler(command,bg,0);
        }
        //resetting back to defaults
        dup2(origOUT,STDOUT_FILENO);
        dup2(origIN,STDIN_FILENO);
    }
    free(input);
    saveH(hq,commandcount);
    return 0;
}

