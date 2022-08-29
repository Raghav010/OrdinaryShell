
#include "prompt.h" 


void shPrompt()
{
    //used to validate error throws
    int ret;
    char* sret;

    //????????maybe dont call getlogin every time??????????
    //getting username
    sret=getlogin();
    if(sret==NULL)
        errorR(1);
    char* username=sret; // returns user name or NULL on failure and errno is set

    /*uid_t uid;
    uid=geteuid();
    struct passwd* user_data;
    user_data=getpwuid(uid);
    printf("%s\n",user_data->pw_name); // returns user name*/

    
    //getting hostname
    struct utsname lap_meta;
    ret=uname(&lap_meta);
    if(ret==-1)
        errorR(1);
    //printf("%s\n",lap_meta.sysname); //return LINUX
    char* hostname=lap_meta.nodename; // returns hostname


    //computing relative path wrt ~
    sret=getcwd(NULL,0);
    if(sret==NULL)
        errorR(1);
    char* wd=sret; //wd is absolute path
    char* pdir=NULL; //pdir is relative path
    int curDirLen=strlen(wd);
    if(homedirLen==-1)
    {
        homedirLen=curDirLen;
    }
    if(curDirLen>=homedirLen)
    {
        pdir=(char*)malloc(curDirLen-homedirLen+2);
        if(pdir==NULL)
            errorR(1);
        pdir[0]='~';
        for(int i=homedirLen,m=1;i<=curDirLen;i++,m++)
        {
            pdir[m]=wd[i];
        }
    }
    else
    {
        pdir=wd;
    }
    free(wd);
    printf("\033[1;37m");
    printf("<%s@%s:%s> ",username,hostname,pdir);
    free(pdir);
    printf("\033[1;35m");
}
