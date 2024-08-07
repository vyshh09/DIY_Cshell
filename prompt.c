#include "headers.h"
extern char *last_command;
extern int last_ex_t;
#define COLOR_RESET "\x1B[0m"
#define COLOR_USERNAME "\x1B[1;32m"  // Green
#define COLOR_HOSTNAME "\x1B[1;34m"  // Blue
#define COLOR_DIRECTORY "\x1B[1;36m" // Cyan
#define COLOR_COMMAND "\x1B[1;35m"   // Magenta
#define COLOR_TIME "\x1B[1;33m"      // Yellow

void get_prompt(char *uname, char *sname, char cwd[], char **home_dir, char relative_dir[], char var[])
{
    getlogin_r(uname, 10000);
    gethostname(sname, 100000);
    getcwd(cwd, MAX_INPUT_SIZE);
    *home_dir = getenv("HOME");
    char curr[1000];
    if (strcmp(cwd, *home_dir) == 0)
        strcpy(relative_dir, "~");
    else if (strncmp(cwd, *home_dir, strlen(*home_dir)) == 0)
    {
        strcpy(relative_dir, "~");
        strcpy(curr, cwd + strlen(var));
        strcat(relative_dir, curr);
    }
    if (last_command != NULL && last_ex_t > 2)
    {
        last_command[strlen(last_command)] = '\0';
        printf("<" COLOR_USERNAME "%s" COLOR_RESET "@" COLOR_HOSTNAME "%s" COLOR_RESET ":" COLOR_DIRECTORY "%s" COLOR_RESET " " COLOR_COMMAND "%s" COLOR_RESET ":%ds> ", uname, sname, relative_dir, last_command, last_ex_t);
    }
    else
        printf("<" COLOR_USERNAME "%s" COLOR_RESET "@" COLOR_HOSTNAME "%s" COLOR_RESET ":" COLOR_DIRECTORY "%s" COLOR_RESET "> ", uname, sname, relative_dir);
}