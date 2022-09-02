#include "ech.h"

int echo()
{
    char* word;
    while(1)
    {
        word=strtok(NULL," \t");
        if(word==NULL)
        {
            printf("\n");
            break;
        }
        printf("%s ",word);
    }
    return 0;

}