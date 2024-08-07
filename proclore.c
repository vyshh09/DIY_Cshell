#include "headers.h"

void get_process_info(int pid)
{
    char filename[MAX_COMMAND_LENGTH];
    FILE *file;
    char line[MAX_COMMAND_LENGTH];

    sprintf(filename, "/proc/%d/status", pid);
    file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Process not found.\n");
        return;
    }

    char process_status[MAX_COMMAND_LENGTH];
    char process_group[MAX_COMMAND_LENGTH];
    char virtual_memory[MAX_COMMAND_LENGTH];
    char executable_path[MAX_COMMAND_LENGTH];

    while (fgets(line, sizeof(line), file))
    {
        char *key = strtok(line, ":");
        char *value = strtok(NULL, ":");

        if (key != NULL && value != NULL)
        {
            key = strtok(key, "\t\n");
            value = strtok(value, "\t\n");

            if (strcmp(key, "State") == 0)
            {
                strcpy(process_status, value);
            }
            else if (strcmp(key, "Pid") == 0)
            {
                strcpy(process_group, value);
            }
            else if (strcmp(key, "VmSize") == 0)
            {
                strcpy(virtual_memory, value);
            }
            else if (strcmp(key, "Name") == 0)
            {
                strcpy(executable_path, value);
            }
        }
    }

    if (process_status[0] == 'R')
    {
        process_status[1] = '+';
        process_status[2] = '\0';
    }

    printf("pid : %d\n", pid);
    printf("Process Status : %s\n", process_status);
    printf("Process Group : %s\n", process_group);
    printf("Virtual memory : %s\n", virtual_memory);
    printf("Executable Path : %s\n", executable_path);

    fclose(file);
}
void read_command(char *command)
{
    if (strcmp(command, "proclore") == 0)
    {
        int shell_pid = getpid();
        get_process_info(shell_pid);
    }
    else
    {
        int pid = atoi(command+8);
        get_process_info(pid);
    }
}