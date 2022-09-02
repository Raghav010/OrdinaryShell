
#include "cdir.h"


char* prevDir=NULL;

//setting the prevDir variable
void setPrevDir()
{
    if(prevDir==NULL)
    {
        prevDir=(char*)malloc(1000);
        if(prevDir==NULL)
        {
            errorR(1);
            return;
        }
    }
    char* d=getcwd(NULL,0);
    if(d==NULL)
    {
        errorR(1);
        return;
    }
    strcpy(prevDir,d);
    free(d);
}

int cd()
{

    int ret=0;

    
    char* relpath=(char*)malloc(1000);
    if(relpath==NULL)
        return errorR(1);
    char* absolutePath=(char*)malloc(1000);
    if(absolutePath==NULL)
        return errorR(1);

    
    char* arg=strtok(NULL," \t");
    if(arg==NULL)
    {
        free(relpath);
        free(absolutePath);
        if(chdir(home)==-1)
            return errorR(1);
        return 0;
    }
    strcpy(relpath,arg);
    arg=strtok(NULL," \t");
    if(arg!=NULL)
    {
        printf("Too many arguments for cd\n");
        free(relpath);
        free(absolutePath);
        return -1;
    }

    char* prevDir2=NULL;
    if(prevDir!=NULL)
    {
        prevDir2=(char*)malloc(1000);
        if(prevDir2==NULL)
            return errorR(1);
        strcpy(prevDir2,prevDir);
    }
    setPrevDir();

    

    if(relpath[0]=='-')
    {
        if(prevDir2==NULL)
        {
            printf("Warning: previous directory not set\n");
        }
        else
        {
            if(chdir(prevDir2)==-1)
                ret=errorR(1);
        }
    }
    else if(relpath[0]=='~')
    {
        sprintf(absolutePath,"%s%s\0",home,relpath+1);
        if(chdir(absolutePath)==-1)
            ret=errorR(1);
    }
    else //this case relpath is absolute path 
    {
        if(chdir(relpath)==-1)
            ret=errorR(1);
    }

    free(relpath);
    free(absolutePath);   
    if(prevDir2!=NULL)
        free(prevDir2);
    if(ret==-1)
        return -1;
    return 0;
}