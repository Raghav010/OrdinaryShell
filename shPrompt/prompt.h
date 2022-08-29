#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "../err/errors.h"

extern int homedirLen;

//prompt for the shell
void shPrompt();
