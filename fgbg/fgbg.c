#include "fgbg.h"


#define MAX_BG_PS 100






//first entry is reserved for foreground message process
char psmsg[1+MAX_BG_PS][64]={0};
int cps=0;
struct bgps bgpids[MAX_BG_PS]; //holds minimal info on background processes
int fgPid;
int fgMsgCount=0;

char* gCommand; //global command variable


void bgpsInit()
{
    for(int i=0;i<MAX_BG_PS;i++)
    {
        bgpids[i].bgpid=-2;
    }
}


//replace gCommand with input
//ensure bgpids[i].bgpid!=-2 checks are happening


//make global variable which has ps messages in it (10 bg messages,or a time fg message)
//make a global variable counting concurrent bg ps / finished bg ps
//find out exit codes for child processes
//print message out with pid stated
//fix signal handling !!!!!


//does not do too many background processes error handling , changes cps ,make sure gCommand is set
int addToBgPs(int pid,int stp)
{
    cps++;
    for(int i=0;i<MAX_BG_PS;i++)
    {
        if(bgpids[i].bgpid==-2)
        {
            bgpids[i].bgpid=pid;
            strcpy(bgpids[i].bgcmd,input);
            bgpids[i].jobnumber=cps;
            bgpids[i].stopped=stp;
            break;
        }
    }
    return 0;
}


//returns -1 if pid could not be found, just changes job numbers and cps
int rmFromBgPs(int pid)
{
    for(int i=0;i<MAX_BG_PS;i++)
    {
        int jnum;
        if(bgpids[i].bgpid==pid)
        {
            jnum=bgpids[i].jobnumber;
            bgpids[i].bgpid=-2;
            cps--;
            //update all job numbers
            for(int k=0;k<MAX_BG_PS;k++)
            {
                if((bgpids[k].bgpid!=-2) && (bgpids[k].jobnumber>jnum))
                {
                    bgpids[k].jobnumber--;
                }
            }
            return 0;
        }
    }
    return -1;
}





//handles if bg process terminated 
void printTer(int signum)
{
    //int bg=0;
    int status;
    int pid=waitpid(-1,&status,WNOHANG); //ensuring a child already exited
    if(pid==-1)
        return;
    if(rmFromBgPs(pid)==-1)
        return;
    /*if(bg==0)
        return;*/
    char* Estatus;
    if(WIFEXITED(status))
        Estatus="normally";
    else
        Estatus="abnormally";
    for(int i=1;i<1+MAX_BG_PS;i++)
    {
        if(psmsg[i][0]=='\0')
        {
            sprintf(psmsg[i],"process with pid %d exited %s\n\0",pid,Estatus); //maybe print command name
            break;
        }
    }
}


void stopFg(int signum)
{
    if(cps==MAX_BG_PS)
    {
        printf("Cant handle more than 100 background processes concurrently\n");
        return;
    }
    //printf("stopping child\n");
    if(kill(fgPid,SIGSTOP)==-1)
    {
        errorR(1);
        return;
    }
    addToBgPs(fgPid,1);
    printf("[%d]         Stopped\n",cps);
    return;
}

void terminateFg(int signum)
{
    //printf("terminating\n");
    if(kill(fgPid,SIGINT)==-1)
    {
        errorR(1);
        return;
    }
    printf("\n");
    return;
}


int cmpCmds(const void* job1,const void* job2)
{
    struct bgps job1_meta=*((struct bgps*)job1);
    struct bgps job2_meta=*((struct bgps*)job2);
    int verdict=strcasecmp(job1_meta.bgcmd,job2_meta.bgcmd);
    if(verdict>0)
        return 1;
    else if(verdict <0)
        return -1;
    else
        return 0;
}



//ignores other stuff takes in only flags , does not throw an error
int printJobs()
{
    int runFlag=0;
    int stopFlag=0;
    while(1)
    {
        char* flags=strtok(NULL," \t");
        if(flags==NULL)
            break;
        if(flags[0]=='-')
        {
            char l;
            int m=1;
            do
            {
                l=flags[m];
                if(l=='r')
                    runFlag=1;
                else if(l=='s')
                    stopFlag=1;
                m++;
            } while (l!='\0');
            
        }
    }
    qsort(bgpids,MAX_BG_PS,sizeof(struct bgps),cmpCmds);
    if(runFlag==stopFlag)
    {
        runFlag=1;
        stopFlag=1;
    }
    for(int i=0;i<MAX_BG_PS;i++)
    {
        if(bgpids[i].bgpid!=-2)
        {
            if(runFlag && !bgpids[i].stopped)
            {
                printf("[%d] Running %s [%d]\n",bgpids[i].jobnumber,bgpids[i].bgcmd,bgpids[i].bgpid);
            }
            else if(stopFlag && bgpids[i].stopped)
            {
                printf("[%d] Stopped %s [%d]\n",bgpids[i].jobnumber,bgpids[i].bgcmd,bgpids[i].bgpid);
            }
        }
    }
    return 0;
}



//considers only running!! jobs
int sendSignal()
{
    int targetJob=atoi(strtok(NULL," \t"));
    int sigNum=atoi(strtok(NULL," \t"));
    if(targetJob==0 || sigNum==0)
    {
        printf("Invalid Argument\n");
        return -1;
    }
    for(int i=0;i<MAX_BG_PS;i++)
    {
        if((bgpids[i].bgpid!=-2) && (bgpids[i].jobnumber==targetJob) && (!bgpids[i].stopped))
        {
            if((sigNum==SIGTSTP) || (sigNum==SIGSTOP) || (sigNum==SIGTTIN) || (sigNum==SIGTTOU))
                bgpids[i].stopped=1;
            int s=kill(bgpids[i].bgpid,sigNum);
            if(s==-1)
                break;
            return 0;
        }
    }
    printf("Invalid job number or signal number\n");
    return -1;
}


//only for stopped bg processes
int bgContinue()
{
    int targetJob=atoi(strtok(NULL," \t"));
    if(targetJob==0)
    {
        printf("Invalid Arguments\n");
        return -1;
    }
    for(int i=0;i<MAX_BG_PS;i++)
    {
        if((bgpids[i].bgpid!=-2) && (bgpids[i].jobnumber==targetJob))
        {
            bgpids[i].stopped=0;
            int s=kill(bgpids[i].bgpid,SIGCONT);
            if(s==-1)
                break;
            return bgpids[i].bgpid;
        }
    }
    printf("Invalid Job Number\n");
    return -1;
}


int bringToFg()
{
    fgPid=bgContinue();
    if(fgPid!=-1)
    {
        rmFromBgPs(fgPid);
        fgbgHandler(NULL,0,1);
        return 0;
    }
    return -1;
}



//justWait parameter is valid only when its a foreground process(this means only set justWait to 1 when bg=0, or undefined behavior occurs)
void fgbgHandler(char* command,int bg,int justWait)
{
    //setting signal dispositions if its a foreground process
    if(!bg)
    {
        if(signal(SIGTSTP,stopFg)==SIG_ERR)
        {
            errorR(1);
            return;
        }
        if(signal(SIGINT,terminateFg)==SIG_ERR)
        {
            errorR(1);
            return;
        }
    }

    //variables
    char* args[32];
    int btime;
    char* arg;
    int argcount;

    if (!justWait)
    {
        if (bg)
        {
            if (cps == MAX_BG_PS)
            {
                printf("Cant handle more than 100 background processes concurrently\n");
                return;
            }
        }

        gCommand = command;
        //int btime;
        //char *args[32];

        args[0] = (char *)malloc(strlen(command) + 1);
        if (args[0] == NULL)
        {
            errorR(1);
            return;
        }
        strcpy(args[0], command);

        //char *arg;
        argcount = 1;
        while (1)
        {
            arg = strtok(NULL, " \t");
            if (arg == NULL)
            {
                args[argcount] = NULL;
                break;
            }
            args[argcount] = (char *)malloc(strlen(arg) + 1);
            if (args[argcount] == NULL)
            {
                errorR(1);
                return;
            }
            strcpy(args[argcount], arg);
            argcount++;
        }
    }
    if(!bg)
        btime=time(NULL);

    
    long int pid;
    if(!justWait)
        pid=fork();
    else
        pid=fgPid;

    if(pid==-1)
    {
        printf("Fork failed\n");
        return;
    }
    
    if(pid==0)
    {
        if(execvp(command,args)==-1)
        {
            exit(-7); //command not found
        }
    }
    else
    {
        if (!justWait)
        {
            for (int i = 0; i < argcount; i++)
            {
                free(args[i]);
            }
        }
        if(!bg)
        {
            fgPid=pid;
            int Estatus;
            waitpid(pid,&Estatus,WUNTRACED);
            int atime=time(NULL);
            if((WIFEXITED(Estatus)) && (WEXITSTATUS(Estatus) == 249)) //decide whether to handle other exit status too
            {
                printf("Error: Command not found\n");
            }
            if((atime-btime)>=1) //print time for fg process that stopped or abnormally terminated?
            {
                sprintf(psmsg[0],"<took %ds>\0",atime-btime);
                fgMsgCount=1;
            }

            //setting back to defualts
            if(signal(SIGTSTP,SIG_IGN)==SIG_ERR)
            {
                errorR(1);
                return;
            }
            if(signal(SIGINT,SIG_IGN)==SIG_ERR)
            {
                errorR(1);
                return;
            }
        }
        if(bg)
        {
            addToBgPs(pid,0);
            printf("[%d] %ld\n",cps,pid);
            //signal(SIGCLD,printTer);
        }
    }
}


void printbgMsg()
{
    for(int i=1;i<1+MAX_BG_PS;i++)
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