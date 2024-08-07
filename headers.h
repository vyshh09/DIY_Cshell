#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <sys/time.h>
#include <termios.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "inputs.h"
#include "pastevents.h"
#include "SystemCommands.h"
#include "proclore.h"
#include "redirection.h"
#include "pipes.h"
#include "activities.h"
#include "signal.h"
#include "fg_bg.h"
#include "ping.h"
#include "neonate.h"
#include "iMan.h"


#define MAX_INPUT_SIZE 1024
#define MAX_COMMANDS 15
#define PAST_EVENTS_FILE "past_events.txt"
#define MAX_COMMAND_LENGTH 100
#define MAX_BACKGROUND_PROCESSES 100
#define DFLT "\x1B[0m" 

struct BackgroundProcess
{
    pid_t pid;
    char name[MAX_COMMAND_LENGTH];
};

struct Entry
{
    char name[MAX_INPUT_SIZE];
    struct stat file_info;
};


#endif