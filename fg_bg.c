#include "headers.h"
extern struct BackgroundProcess bgProcesses[MAX_COMMANDS];
extern int numBgProcesses;
void handle_fg(char *command)
{
    int pid;
    if (sscanf(command, "fg %d", &pid) == 1)
    {
        // Find the process with the given PID
        bool found = false;
        for (int i = 0; i < numBgProcesses; i++)
        {
            if (bgProcesses[i].pid == pid)
            {
                found = true;
                bringToForeground(pid);
                printf("[%d] : %s - Running\n", bgProcesses[i].pid, bgProcesses[i].name);
                break;
            }
        }

        if (!found)
        {
            printf("No such process found\n");
        }
    }
}

void handle_bg(char *command)
{
    int pid;
    if (sscanf(command, "bg %d", &pid) == 1)
    {
        // Find the process with the given PID
        bool found = false;
        for (int i = 0; i < numBgProcesses; i++)
        {
            if (bgProcesses[i].pid == pid)
            {
                found = true;
                resumeBackgroundProcess(pid);
                printf("[%d] : %s - Running (in the background)\n", bgProcesses[i].pid, bgProcesses[i].name);
                break;
            }
        }

        if (!found)
        {
            printf("No such process found\n");
        }
    }
}

int bringToForeground(pid_t pid)
{
    if (pid > 0)
    {
        // Send a SIGCONT signal to the process to resume it if stopped
        kill(pid, SIGCONT);

        // Wait for the process to complete
        int status;
        waitpid(pid, &status, WUNTRACED);
    }
}
int resumeBackgroundProcess(pid_t pid)
{
    if (pid > 0)
    {
        // Send a SIGCONT signal to the process to resume it
        kill(pid, SIGCONT);
    }
}