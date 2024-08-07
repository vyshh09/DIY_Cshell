#include "headers.h"
extern char *last_command;
extern int last_ex_t;
extern struct BackgroundProcess bgProcesses[MAX_COMMANDS];
extern int numBgProcesses;
void executeInBackground(char *command)
{
    size_t commandLength = strlen(command);
    if (command[commandLength - 2] == '&')
    {
        command[commandLength - 2] = '\0';
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        return;
    }
    else if (pid == 0)
    {
        execlp("/bin/sh", "/bin/sh", "-c", command, (char *)NULL);
        perror("exec");
        exit(1);
    }
    else
    {
        strncpy(bgProcesses[numBgProcesses].name, command, MAX_COMMAND_LENGTH - 1);
        bgProcesses[numBgProcesses].name[MAX_COMMAND_LENGTH - 1] = '\0';
        bgProcesses[numBgProcesses].pid = pid;
        (numBgProcesses)++;

        printf("%d\n", pid);
    }
}

void exf(const char *co)
{
    pid_t pid;
    int status;
    struct timeval start_time, end_time;

    pid = fork();
    if (pid == 0)
    {
        execlp("/bin/sh", "/bin/sh", "-c", co, (char *)NULL);
        perror("exec");
        exit(1);
    }
    else if (pid > 0)
    {
        gettimeofday(&start_time, NULL);
        waitpid(pid, &status, 0);
        gettimeofday(&end_time, NULL);
        double ex_t = (double)(end_time.tv_sec - start_time.tv_sec) +
                      (double)(end_time.tv_usec - start_time.tv_usec) / 1000000.0;

        if (ex_t > 2.00)
        {
            printf("# takes : %.0fs\n", ex_t);
            last_command = strdup(co);
            last_ex_t = (int)ex_t;
        }
        else
        {
            last_ex_t = 0;
        }
    }
    else
    {
        perror("fork error");
    }
}

void handleBackgroundProcessTermination()
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        int index = -1;
        for (int i = 0; i < numBgProcesses; i++)
        {
            if (bgProcesses[i].pid == pid)
            {
                index = i;
                break;
            }
        }

        if (index >= 0)
        {
            if (WIFEXITED(status))
            {
                printf("%s (PID: %d) exited normally\n", bgProcesses[index].name, pid);
            }
            else
            {
                printf("%s (PID: %d) exited abnormally\n", bgProcesses[index].name, pid);
            }

            for (int i = index; i < numBgProcesses - 1; i++)
            {
                bgProcesses[i] = bgProcesses[i + 1];
            }
            (numBgProcesses)--;
        }
    }
}
