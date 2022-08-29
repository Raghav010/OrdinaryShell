#include "print_wd.h"

void wd()
{
    //error handling var
    char* sret;

    sret=getcwd(NULL,0);
    if(sret==NULL)
        errorR(1);
    char* wd=sret; //wd is absolute path
    printf("%s\n",wd);
}