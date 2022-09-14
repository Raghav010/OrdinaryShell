#include "discover.h"




int discover(char* parent,char* curdir,char* tofind,int* flags)
{
    if(parent==NULL)
    {
        parent=(char*)malloc(1024);
        if(parent==NULL)
            return errorR(1);
        parent[0]='\0';
    }

    
    strcat(parent,curdir);
    strcat(parent,"/");
    int curPathlen=strlen(parent);


    if(((!flags[2]) || (!strcmp(tofind,curdir))) && flags[1])
        printf("%s\n",parent);

    //taking car of ~
    char* validPath;
    int tilda=0;
    if(parent[0]=='~')
    {
        validPath=(char*)malloc(1024);
        if(validPath==NULL)
            return errorR(1);
        sprintf(validPath,"%s%s\0",home,parent+1);
        tilda=1;
    }
    else
    {
        validPath=parent;
    }
    DIR* dh=opendir(validPath);
    if(dh==NULL)
    {
        errorR(1);
        if(tilda==1)
            free(validPath);
        return -1;
    }
    if(tilda==1)
        free(validPath);

    //going through the directory
    while(1)
    {
        struct dirent* entry=readdir(dh);
        if(entry==NULL)
            break;
        if(entry->d_type == DT_REG)
        {
            if(((!flags[2]) || (!strcmp(tofind,entry->d_name))) && flags[0])
                printf("%s%s\n",parent,entry->d_name);
        }
        if(entry->d_type == DT_DIR)
        {
            if(strcmp(entry->d_name,".") && strcmp(entry->d_name,".."))
            {
                discover(parent,entry->d_name,tofind,flags);
            }
            
        }
        parent[curPathlen]='\0';
    }
    closedir(dh);

    return 0;
}



//make a default directory with curdir
//make a flags list for -d and -f(default 0) flags[0] is -f flags[1] is -d flags[2] is file?
//if both -d and -f arent ther then put both -d and -f to 1



void parseInp()
{
    char* startDir=(char*)malloc(512);
    if(startDir==NULL)
    {
        errorR(1);
        return;
    }
    startDir[0]='.';
    startDir[1]='\0';
    int flags[3]={0,0,0};  //-f -d and file?
    char tofind[20];

    while(1)
    {
        char* arg=strtok(NULL," \t");
        if(arg==NULL)
            break;
        if(arg[0]=='-')
        {
            int m=1;
            do
            {
                if(arg[m]=='f')
                    flags[0]=1;
                else if(arg[m]=='d')
                    flags[1]=1;
                else
                {
                    printf("Invalid flags\n");
                    free(startDir);
                    return;
                }
                m++;
            }while(arg[m]!='\0');
        }
        else if(arg[0]=='"')
        {
            flags[2]=1;
            sscanf(arg+1,"%[^\"]",tofind);
        }
        else
        {
            strcpy(startDir,arg);
        }
    }

    if((!flags[0]) && (!flags[1]))
    {
        flags[0]=1;
        flags[1]=1;
    }

    char* parent=NULL;
    discover(parent,startDir,tofind,flags);
    free(parent);
    free(startDir);
}