#include "print_wd.h"

int wd()
{
    //error handling var
    char* sret;
    
    sret=getcwd(NULL,0);
    if(sret==NULL)
        return errorR(1);
    char* wd=sret; //wd is absolute path
    printf("%s\n",wd);
    free(wd); 
    return 0; 
}