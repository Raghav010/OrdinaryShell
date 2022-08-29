
#include "errors.h"

void errorR(int criticalOp)
{
    //maybe add functionality to include a message
    perror("\033[1;31m");
    if(criticalOp==1)
    {
        exit(7);
    }
    printf("\033[1;35m");
}