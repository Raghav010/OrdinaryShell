#include "fgbg.h"



//first entry is reserved for foreground message process
char psmsg[21][100]={0};
int cps=0;
int bgpids[20];
int fgMsgCount=0;



void bgpidsInit()
{
    for(int i=0;i<20;i++)
    {
        bgpids[i]=-2;
    }
}


//make global variable which has ps messages in it (10 bg messages,or a time fg message)
//make a global variable counting concurrent bg ps / finished bg ps
//find out exit codes for child processes
//print message out with pid stated
//fix signal handling !!!!!



void printTer(int signum)
{
    int bg=0;
    int status;
    int pid=wait(&status);
    for(int i=0;i<20;i++)
    {
        if(bgpids[i]==pid)
        {
            bg=1;
            bgpids[i]=-2;
            cps--;
            break;
        }
    }
    if(bg==0)
        return;
    char* Estatus;
    if(WIFEXITED(status))
        Estatus="normally";
    else
        Estatus="abnormally";
    for(int i=1;i<21;i++)
    {
        if(psmsg[i][0]=='\0')
        {
            sprintf(psmsg[i],"process with pid %d exited %s\n\0",pid,Estatus);
            break;
        }
    }
}



void fgbgHandler(char* command,int bg)
{
    int btime;
    char* args[32];

    args[0]=(char*)malloc(strlen(command)+1);
    if(args[0]==NULL)
    {
        errorR(1);
        return;
    }
    strcpy(args[0],command);


    char* arg;
    int argcount=1;
    while(1)
    {
        arg=strtok(NULL," \t");
        if(arg==NULL)
        {
            args[argcount]=NULL;
            break;
        }
        args[argcount]=(char*)malloc(strlen(arg)+1);
        if(args[argcount]==NULL)
        {
            errorR(1);
            return;
        }
        strcpy(args[argcount],arg);
        argcount++;
    }

    if(!bg)
        btime=time(NULL);
    long int pid=fork(); 
    if(pid==0)
    {
        if(execvp(command,args)==-1)
        {
            exit(-7); //command not found
        }
    }
    else
    {
        for(int i=0;i<argcount;i++)
        {
            free(args[i]);
        }
        if(!bg)
        {
            int Estatus;
            waitpid(pid,&Estatus,0);
            int atime=time(NULL);
            if((WIFEXITED(Estatus)) && (WEXITSTATUS(Estatus) == 249)) 
            {
                printf("Error: Command not found\n");
            }
            if((atime-btime)>=1)
            {
                sprintf(psmsg[0],"<took %ds>\0",atime-btime);
                fgMsgCount=1;
            }
        }
        if(bg)
        {
            cps++;
            if(cps==21)
            {
                printf("Cant handle more than 20 background processes concurrently\n");
                cps--;
                return;
            }
            printf("[%d] %ld\n",cps,pid);
            for(int i=0;i<20;i++)
            {
                if(bgpids[i]==-2)
                {
                    bgpids[i]=pid;
                    break;
                }
            }
            //signal(SIGCLD,printTer);
        }
    }
}


void printbgMsg()
{
    for(int i=1;i<21;i++)
    {
        if(psmsg[i][0]=='\0')
            continue;
        fprintf(stderr,"%s\n",psmsg[i]);
        psmsg[i][0]='\0';
    }
}


void printfgMsg()
{
    if(fgMsgCount)
    {
        printf("\033[34;1m");
        printf("%s ",psmsg[0]);
        printf("\033[35;1m");
        fgMsgCount=0;
    }
}