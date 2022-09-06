#include "history.h"


int min(int a,int b)
{
    return (a<b)?a:b;
}

int max(int a,int b)
{
    return (a>b)?a:b;
}



char** createHq()
{
    char** q=(char**)malloc(20*sizeof(char*));
    return q;
}


//returns command count
int loadH(char** q)
{
    int commandcount=0;
    FILE* hf=fopen(".history.txt","r");
    if(hf==NULL)
        return commandcount;
    while(1)
    {
        char* cmd=(char*)malloc(512);
        if(fscanf(hf,"%[^\n]",cmd)==-1)
        {
            free(cmd);
            break;
        }
        fgetc(hf); //reads the newline
        q[commandcount]=cmd;
        commandcount++;
    }
    fclose(hf);
    return commandcount;
}

//do not pass a malloced string to this , increments command count
void addcmd(int* commandcount,char** q,char* cmd)
{
    int commandC=*commandcount;
    int flag=1;
    if(commandC==0)
    {
        char* ncmd=(char*)malloc(512);
        strcpy(ncmd,cmd);
        q[(commandC)]=ncmd;
        flag=0;   
    }
    if((commandC)>=20)
    {
        if(strcmp(q[(commandC-1)%20],cmd)!=0)
        {
            strcpy(q[(commandC)%20],cmd);
            flag=0;
        }
    }
    else
    {
        if(commandC!=0 && strcmp(q[commandC-1],cmd)!=0)
        {
            char* ncmd=(char*)malloc(512);
            strcpy(ncmd,cmd);
            q[(commandC)]=ncmd;
            flag=0;
        }
    }
    if(flag==0)
        (*commandcount)++;
}


void saveH(char** q,int commandcount)
{
    if(commandcount==0)
        return;
    FILE* fh=fopen(".history.txt","w");
    if(commandcount>20)
    {
        for(int i=0;i<20;i++)
        {
            fprintf(fh,"%s\n",q[(commandcount+i)%20]);
        }
    }
    else
    {
        for(int i=0;i<commandcount;i++)
        {
            fprintf(fh,"%s\n",q[i]);
        }
    }
    fclose(fh);
}

void printH(char** q,int commandcount)
{
    if(commandcount>=20)
    {
        int start=((commandcount-10)%20);
        for(int i=0;i<10;i++)
        {
            printf("%s\n",q[(start+i)%20]);
        }
    }
    else
    {
        int start=max(0,commandcount-10);
        for(int i=start;i<min(commandcount,start+10);i++)
        {
            printf("%s\n",q[i]);
        }
    }
}