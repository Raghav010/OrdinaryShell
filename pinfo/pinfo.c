#include "pinfo.h"


int printPinfo()
{
    int ipid;
    char* pid=strtok(NULL," \t");
    if(pid==NULL)
    {
        ipid=getpid();
    }
    else
    {
        ipid=atoi(pid);
    }


    //getting path of stat file
    char statPath[100];
    sprintf(statPath,"/proc/%d/stat\0",ipid);

    //getting variables ready
    char state;
    char absPath[200]={0};
    unsigned long int vm;
    int tpgid;

    FILE* fh=fopen(statPath,"r");
    if(fh==NULL)
        return errorR(1);

    fscanf(fh,"%*d %*s %c %*d %*d %*d %*d %d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*ld %*ld %*ld %*ld %*llu %lu",&state,&tpgid,&vm);

    fclose(fh);

    //getting executable path
    sprintf(statPath,"/proc/%d/exe\0",ipid);
    if(readlink(statPath,absPath,200)==-1)
        return errorR(1);


    //getting relpath
    int freeflag=0;
    char* relpath;
    int curDirLen=strlen(absPath);
    int homedirLen=strlen(home);
    if(strncmp(absPath,home,homedirLen)==0)
    {
        relpath=(char*)malloc(curDirLen-homedirLen+2);
        if(relpath==NULL)
            return errorR(1);
        relpath[0]='~';
        for(int i=homedirLen,m=1;i<=curDirLen;i++,m++)
        {
            relpath[m]=absPath[i];
        }
        freeflag=1;
    }
    else
    {
        relpath=absPath;
    }



    //printing
    printf("pid : %d\n",ipid);
    printf("process status : %c",state);
    if(ipid==tpgid)
    {
        printf("+");
    }
    printf("\n");
    printf("memory : %lu\n",vm);
    printf("executable path : %s\n",relpath);
    if(freeflag==1)
        free(relpath);

    return 0;
}