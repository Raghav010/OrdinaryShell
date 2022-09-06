#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../err/errors.h"
#include "../gvars.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>



struct statN
{
    struct stat* stats;
    char* name;
};



int lsg();