#include "headers.h"
extern int identifier;
void execute_command(const char *command, int run_in_background, int *count, char *cwd, const char *home_dir, const char *var, char prev_dir[MAX_INPUT_SIZE], char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH])
{
    if (strncmp(command, "peek", 4) == 0)
    {
        char cha[MAX_INPUT_SIZE];
        strcpy(cha, command);
        char *args = cha + 4;
        while (*args == ' ')
            args++;

        int show_hidden = 0;
        int show_details = 0;
        while (*args == '-')
        {
            args++;

            while (*args != ' ' && *args != '\0')
            {
                if (*args == 'a')
                    show_hidden = 1;
                else if (*args == 'l')
                    show_details = 1;
                else
                {
                    break;
                }
                args++;
            }
            while (*args == ' ')
                args++;
        }

        char *path = args;
        if (*path == '\0')
            path = cwd;

        if (is_symbol(path))
            path = expand_path(path, home_dir, cwd, var);
        list_directory(path, show_hidden, show_details);
    }
    else if (strncmp(command, "warp", 4) == 0)
    {
        char jan[MAX_INPUT_SIZE];
        strcpy(jan, command);
        char *args = jan + 4;
        while (*args == ' ')
            args++;

        char *path = strtok(args, " ");
        if (path != NULL && path[0] == '~')
        {
            chdir(var);
            strcpy(cwd, var);
            if (path != NULL)
            {
                char temp[MAX_INPUT_SIZE];
                strcpy(temp, var);
                strcat(temp, path + 1);
                char *expanded_path = expand_path(path + 1, home_dir, temp, var);
                if (chdir(temp) != 0)
                {
                    perror("warp");
                }
                if (chdir(temp) == 0)
                {
                    strcpy(cwd, expanded_path);
                    printf("%s\n", temp);
                }
                free(expanded_path);
            }
        }
        else
        {
            while (path != NULL)
            {
                if (strcmp(path, "..") == 0)
                {
                    chdir("..");
                    getcwd(cwd, MAX_INPUT_SIZE);
                }
                else if (strcmp(path, "-") == 0)
                {
                    if (strlen(prev_dir) > 0)
                    {
                        chdir(prev_dir);
                        strcpy(cwd, prev_dir);
                    }
                    printf("%s\n", cwd);
                }
                else
                {
                    strcpy(prev_dir, cwd);
                    char *expanded_path = expand_path(path, home_dir, cwd, var);
                    char new[MAX_INPUT_SIZE];
                    strcpy(new, cwd);
                    strcat(new, "/");
                    strcat(new, expanded_path);
                    if (chdir(expanded_path) == 0)
                    {
                        printf("%s\n", new);
                        strcpy(cwd, expanded_path);
                    }
                    else
                    {
                        strcpy(cwd, expanded_path);
                    }
                    free(expanded_path);
                }
                path = strtok(NULL, " ");
            }
        }
    }
    else if (strncmp(command, "pastevents", 10) == 0)
    {
        handle_pastevents_command(command, past_events, count, cwd, home_dir, var, prev_dir);
    }
    else
    {

        char *inpu[MAX_INPUT_SIZE];
        int inpu_count = 0;
        char *tokener = strtok((char *)command, " ");
        while (tokener != NULL)
        {
            inpu[inpu_count++] = tokener;
            tokener = strtok(NULL, " ");
        }
        inpu[inpu_count] = NULL;

        pid_t child_pid;

        child_pid = fork();

        if (child_pid < 0)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (child_pid == 0 && run_in_background == 0)
        {
            execvp(inpu[0], inpu);
            perror(inpu[0]);
            exit(EXIT_FAILURE);
        }
        else if (child_pid > 0)
        {
            if (run_in_background == 1)
            {
                printf("Child process (PID %d) running in the background.\n", getpid());
            }
            wait(NULL);
        }
    }
}

void parse_and_execute(const char *input, int *count, const char *cwd, const char *home_dir, const char *var, char prev_dir[MAX_INPUT_SIZE], char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH])
{
    if (strncmp(input, "peek", 4) != 0 && strncmp(input, "pastevents", 10) != 0 && strncmp(input, "warp", 4) != 0 && strncmp(input, "peek", 4) != 0)
    {
        identifier = 1;
    }
    char *commands[MAX_COMMANDS];
    int command_count = 0;
    char temp[10000];
    strcpy(temp, input);
    char *token = strtok((char *)input, "&");
    if (strcmp(token, temp) != 0)
    {
        while (token != NULL && command_count < MAX_COMMANDS)
        {
            commands[command_count++] = token;
            token = strtok(NULL, "&");
        }

        for (int i = 0; i < command_count; i++)
        {
            if (i == 0)
            {
                if (identifier == 1)
                {
                    executeInBackground(commands[i]);
                }
                else
                    execute_command(commands[i], 1, count, (char *)cwd, home_dir, var, prev_dir, past_events);
            }
            else
            {
                if (identifier == 1)
                {
                    exf(commands[i]);
                }
                else
                    execute_command(commands[i], 0, count, (char *)cwd, home_dir, var, prev_dir, past_events);
            }
        }
    }
    else
    {
        command_count = 0;
        token = strtok((char *)input, ";");
        if (strcmp(token, temp) != 0)
        {
            while (token != NULL && command_count < MAX_COMMANDS)
            {
                commands[command_count++] = token;
                token = strtok(NULL, ";");
            }

            for (int i = 0; i < command_count; i++)
            {
                if (identifier == 1)
                {
                    exf(commands[i]);
                }
                else
                    execute_command(commands[i], 0, count, (char *)cwd, home_dir, var, prev_dir, past_events);
            }
        }
        else
        {
            if (identifier == 1)
            {
                exf(temp);
            }
            else
                execute_command(temp, 0, count, (char *)cwd, home_dir, var, prev_dir, past_events);
        }
    }
}
