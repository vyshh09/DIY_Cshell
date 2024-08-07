#include "headers.h"

void load_past_events(char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH], int *count)
{
    FILE *file = fopen(PAST_EVENTS_FILE, "r");
    *count = 0;

    if (file)
    {
        while ((*count < MAX_COMMANDS) && fgets(past_events[*count], MAX_COMMAND_LENGTH, file))
        {
            if (strcmp(past_events[*count], "\n") != 0 && strncmp(past_events[*count], "pastevents", 10) != 0 && strncmp(past_events[*count], "pastevents execute", 18) != 0)
            {
                past_events[*count][strcspn(past_events[*count], "\n")] = '\0';
                (*count)++;
            }
        }
        fclose(file);
    }
}

void save_past_events(char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH], int count)
{
    FILE *file = fopen(PAST_EVENTS_FILE, "w");

    if (file)
    {
        for (int i = 0; i < count; i++)
        {
            if (strncmp(past_events[i], "pastevents", 10) != 0 && strncmp(past_events[i], "pastevents execute", 18) != 0 && strcmp(past_events[i], "\n") != 0)
            {
                fprintf(file, "%s\n", past_events[i]);
            }
        }
        fclose(file);
    }
}

void print_events(char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH], int *count)
{
    for (int i = 0; i < *count; i++)
    {
        printf(" %s\n", past_events[i]);
    }
}
void handle_pastevents_command(const char *command, char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH], int *count, const char *cwd, const char *home_dir, const char *var, char prev_dir[MAX_INPUT_SIZE])
{
    if (strcmp(command, "pastevents purge") == 0)
    {
        *count = 0;
        save_past_events(past_events, *count);
    }
    else if (strncmp(command, "pastevents execute", 18) == 0)
    {
        int n;
        if (sscanf(command, "pastevents execute %d", &n) == 1 && n > 0)
        {
            if (n <= *count && n > 0)
            {
                parse_and_execute(past_events[*count - n], count, cwd, home_dir, var, prev_dir, past_events);
                if (*count == MAX_COMMANDS)
                {
                    for (int i = 0; i < *count - 1; i++)
                    {
                        strcpy(past_events[i], past_events[i + 1]);
                    }
                    (*count)--;
                }
                strcpy(past_events[*count], past_events[*count - n]);
                (*count)++;
                save_past_events(past_events, *count);
            }
            else
            {
                printf("Error: Invalid command number.\n");
            }
        }
        else
        {
            printf("Error: Invalid input format.\n");
        }
    }
    else if (strcmp(command, "pastevents") == 0)
    {
        print_events(past_events, count);
    }
    else
    {
        printf("Error: Command not found.\n");
    }
}