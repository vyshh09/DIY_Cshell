# include "headers.h"
extern struct BackgroundProcess bgProcesses[MAX_COMMANDS];
extern int numBgProcesses;
void printActivities()
{
    printf("Activities:\n");

    for (int i = 0; i < numBgProcesses - 1; i++)
    {
        for (int j = i + 1; j < numBgProcesses; j++)
        {
            if (strcmp(bgProcesses[i].name, bgProcesses[j].name) > 0)
            {
                struct BackgroundProcess temp = bgProcesses[i];
                bgProcesses[i] = bgProcesses[j];
                bgProcesses[j] = temp;
            }
        }
    }
    for (int i = 0; i < numBgProcesses; i++)
    {
        printf("[%d] : %s - ", bgProcesses[i].pid, bgProcesses[i].name);
        printProcessStatus(bgProcesses[i].pid);
    }
}

int printProcessStatus(pid_t pid)
{
    char statusPath[64];
    snprintf(statusPath, sizeof(statusPath), "/proc/%d/stat", pid);

    FILE *file = fopen(statusPath, "r");
    if (file)
    {
        char line[256];
        if (fgets(line, sizeof(line), file))
        {
            char state;
            if (sscanf(line, "%*d %*s %c", &state) == 1)
            {
                if (state == 'T')
                {
                    printf("Stopped\n");
                }

                else
                {
                    printf("Running\n");
                }
            }
        }
        fclose(file);
    }

    else
    {
        perror("fopen");
    }
}
