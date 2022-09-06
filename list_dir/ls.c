#include "ls.h"

//first parse all tokens
//categorize tokens into flags(invalid flags return -1)(set flags in a flag array),files,and directories free these mallocs!!!
//print files first then directories with directory entries sorted
//make functions for printing directories and files
//the functions should have access to the flag array(global??)


//dynamic memory allocation
//natural sorting and not ascii sort
//colourize outputs
//make printing pretty(add newlines between multiple files and directories)
//maybe switch to storing names instead of stats(important!!!!!!)

int cmp(const void* file1,const void* file2)
{
    struct statN* file1ptr=*((struct statN**)file1);
    struct statN* file2ptr=*((struct statN**)file2);
    int verdict=strcmp(file1ptr->name,file2ptr->name);
    if(verdict>0)
        return 1;
    else if(verdict<0)
        return -1;
    else
        return 0;
}

int cmpname(const void* file1,const void* file2)
{
    char* file1ptr=*((char**)file1);
    char* file2ptr=*((char**)file2);
    int verdict=strcmp(file1ptr,file2ptr);
    if(verdict>0)
        return 1;
    else if(verdict<0)
        return -1;
    else
        return 0;
}

//prints perms in rwx format
void printPerms(mode_t modes)
{
    if(modes & S_IRUSR)
        printf("r");
    else
        printf("-");
    if(modes & S_IWUSR)
        printf("w");
    else
        printf("-");
    if(modes & S_IXUSR)
        printf("x");
    else
        printf("-");
    if(modes & S_IRGRP)
        printf("r");
    else
        printf("-");
    if(modes & S_IWGRP)
        printf("w");
    else
        printf("-");
    if(modes & S_IXGRP)
        printf("x");
    else
        printf("-");
    if(modes & S_IROTH)
        printf("r");
    else
        printf("-");
    if(modes & S_IWOTH)
        printf("w");
    else
        printf("-");
    if(modes & S_IXOTH)
        printf("x");
    else
        printf("-");

}

//prints month day HH:MM
void printMtime(struct timespec timeM)
{
    char* wordTime=ctime(&timeM.tv_sec);
    char* tok=strtok(wordTime," \t");
    for(int i=1;i<4;i++)
    {
        char* tok=strtok(NULL," \t");
        if(i==1)
        {
            printf("%s ",tok);
        }
        else if(i==2)
        {
            printf("%2s ",tok);
        }
        else if(i==3)
        {
            tok[5]='\0';
            printf("%s",tok);
        }
    }
}




//prints the file in formatted form according to the -l flag (if -l flag isnt there space terminated)
void printFilef(struct statN* file,int* flags)
{
    if(flags[0])
    {
        struct stat* fileStat=file->stats;
        if(S_ISDIR(fileStat->st_mode))
            printf("d");
        else
            printf("-");
        printPerms(fileStat->st_mode);
        printf(" ");
        printf("%3ld ",fileStat->st_nlink);
        printf("%s ",getpwuid(fileStat->st_uid)->pw_name);
        printf("%s ",getpwuid(fileStat->st_gid)->pw_name);
        printf("%7ld ",fileStat->st_size);
        printMtime(fileStat->st_mtim);
        printf(" ");
    }
    printf("%s",file->name);
    if(flags[0])
        printf("\n");
    else
        printf(" ");
}





void printFiles(int* flags,struct statN** fileArr,int fileCount)
{
    if(fileCount==0)
        return;
    qsort(fileArr,fileCount,sizeof(struct statN*),cmp); //fix ascii sorting vs natural sorting
    for(int i=0;i<fileCount;i++)
    {
        printFilef(fileArr[i],flags);
        free(fileArr[i]->name);
        free(fileArr[i]->stats);
        free(fileArr[i]);
    }
    if(!flags[0])
        printf("\n");
    free(fileArr);
}

//add -a flag support
void printDirContents(int* flags,char* dirName)
{
    DIR* dh=opendir(dirName);
    char** fileNames=(char**)malloc(512*sizeof(char*)); //add support for more files
    int m=0;
    while(1)
    {
        struct dirent* bfilestat=readdir(dh);
        if(bfilestat==NULL)
        {
            break;
        }
        else
        {
            if(!flags[1] && bfilestat->d_name[0]=='.') //taking care of -a flag
                continue;
            char* filename=(char*)malloc(256); //add dynamic memory allocation later
            strcpy(filename,bfilestat->d_name);
            fileNames[m]=filename;
        }
        m++;
    }
    closedir(dh);
    qsort(fileNames,m,sizeof(char*),cmpname);
    for(int i=0;i<m;i++)
    {
        struct statN filestat;
        struct stat  fstats;
        filestat.stats=&fstats;
        filestat.name=fileNames[i];
        stat(fileNames[i],filestat.stats);
        printFilef(&filestat,flags);
        free(fileNames[i]);
    }
    free(fileNames);
}


//losing memory here
void printDir(int* flags,struct statN** dirArr,int dircount,int filecount)
{
    //no params were given
    if(dircount==0 && filecount==0)
    {
        printDirContents(flags,".");
        if(!flags[0])
            printf("\n");
        return;
    }
    if(dircount==0 && filecount!=0)
        return;
    qsort(dirArr,dircount,sizeof(struct statN*),cmp);
    for(int i=0;i<dircount;i++)
    {
        char* dirname=dirArr[i]->name;
        if((dircount-1) || filecount)
            printf("%s:\n",dirname);
        printDirContents(flags,dirname);
        if(!flags[0])  //newline after every directory entry without -l flag
            printf("\n");
        free(dirArr[i]->stats);
        free(dirArr[i]->name);
        free(dirArr[i]);
    }
    free(dirArr);
    
}





//print files
//print directories
int lsg()
{
    //flags[0]=-l flags[1]=-a
    int flags[2]={0,0};
    int filecount=0;
    int dircount=0;

    //max file,directory arguments is 128
    struct statN** fileArr=(struct statN**)malloc(128*sizeof(struct statN*));
    if(fileArr==NULL)
        return errorR(1);
    struct statN** dirArr=(struct statN**)malloc(128*sizeof(struct statN*));
    if(dirArr==NULL)
        return errorR(1);


    //scanning arguments
    while(1)
    {
        char* tok=strtok(NULL," \t");
        if(tok==NULL)
            break;

        //checking if its a flag
        if(tok[0]=='-')
        {
            int ix=1;
            do
            {
                if(tok[ix]=='l')
                {
                    flags[0]=1;
                }
                else if(tok[ix]=='a')
                {
                    flags[1]=1;
                }
                else
                {
                    printf("Invalid Flag\n");
                    return -1;
                }
                ix++;
            }while(tok[ix]!='\0');
        }
        else  //checking if its a normal argument
        {
            //parsing the ~
            char* absolutePath=(char*)malloc(1024); //add dynamic memory allocation later
            if(absolutePath==NULL)
                return errorR(1);
            if(tok[0]=='~')
            {
                sprintf(absolutePath,"%s%s\0",home,tok+1);
            }
            else
            {
                strcpy(absolutePath,tok);
            }

            //categorizing the files as normal or dirctories
            struct statN* cStat=(struct statN*)malloc(sizeof(struct statN));
            struct stat* arg=(struct stat*)malloc(sizeof(struct stat));

            if(stat(absolutePath,arg)==-1)
                return errorR(1);
        
            cStat->name=absolutePath;
            cStat->stats=arg;

            if(S_ISREG(arg->st_mode))
            {
                fileArr[filecount]=cStat;
                filecount++;
            }
            else if(S_ISDIR(arg->st_mode))
            {
                dirArr[dircount]=cStat;
                dircount++;
            }

        }
    }

    printFiles(flags,fileArr,filecount);
    printDir(flags,dirArr,dircount,filecount);
    return 0;
}