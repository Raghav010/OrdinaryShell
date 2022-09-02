
#include "errors.h"

int errorR(int urgencyL)
{
    //maybe add functionality to include a message
    perror("\033[1;31m");
    if(urgencyL==1)
    {
        return -1;
    }
    if(urgencyL==2)
    {
        exit(-1);
    }
    printf("\033[1;35m");
}