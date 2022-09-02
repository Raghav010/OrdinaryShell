#include "prompt.h" 

int homedirLen=-1;
char* home;

int shPrompt()
{
    //used to validate error throws
    int ret;
    char* sret;

    //????????maybe dont call getlogin every time??????????
    //getting username
    sret=getlogin();
    if(sret==NULL)
        return errorR(1);
    char* username=sret; // returns user name or NULL on failure and errno is set


    
    //getting hostname
    struct utsname lap_meta;
    ret=uname(&lap_meta);
    if(ret==-1)
        return errorR(1);
    char* hostname=lap_meta.sysname; // returns LINUX


    //computing relative path wrt ~
    sret=getcwd(NULL,0);
    if(sret==NULL)
        return errorR(1);
    char* wd=sret; //wd is absolute path

    char* pdir=NULL; //pdir is relative path
    int curDirLen=strlen(wd);
    if(homedirLen==-1)
    {
        homedirLen=curDirLen;
        //saving home directory 
        home=(char*)(malloc(homedirLen+1));
        if(home==NULL)
            return errorR(1);
        strcpy(home,wd);
    }
    if(strncmp(wd,home,homedirLen)==0)
    {
        pdir=(char*)malloc(curDirLen-homedirLen+2);
        if(pdir==NULL)
            return errorR(1);
        pdir[0]='~';
        for(int i=homedirLen,m=1;i<=curDirLen;i++,m++)
        {
            pdir[m]=wd[i];
        }
        free(wd);
    }
    else
    {
        pdir=wd;
    }

    printf("\033[1;37m");
    printf("<%s@%s:%s> ",username,hostname,pdir);
    free(pdir);
    printf("\033[1;35m");
    return 0;
}
