#include "headers.h"

int identifier = 0;
char *last_command = NULL;
int last_ex_t = 0;
pid_t fgProcessPid = 0;

struct termios original_termios;
struct BackgroundProcess bgProcesses[MAX_COMMANDS];
int numBgProcesses = 0;
int main()
{
    char command[10000];
    char inp[10000];
    char *uname = malloc(sizeof(char) * 10000);
    char *sname = malloc(sizeof(char) * 100000);
    char cwd[MAX_INPUT_SIZE];
    char pwd[MAX_INPUT_SIZE];
    char prev_dir[MAX_INPUT_SIZE];
    char *home_dir;
    char var[MAX_INPUT_SIZE];
    char relative_dir[10000];
    getcwd(cwd, MAX_INPUT_SIZE);
    strcpy(var, cwd);
    char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH];
    int count = 0;
    load_past_events(past_events, &count);
    int flag = 0;
    signal(SIGINT, handleSIGINT);
    struct termios original_termios;
    tcgetattr(STDIN_FILENO, &original_termios);

    while (1)
    {
        handleBackgroundProcessTermination();
        get_prompt(uname, sname, cwd, &home_dir, relative_dir, var);

        if (fgets(command, 10000, stdin) == NULL)
        {
            break;
        }
        command[strcspn(command, "\n")] = 0;
        strcpy(inp, command);
        if (strncmp(command, "pastevents", 10) != 0 && strncmp(command, "pastevents execute", 18) != 0 && strstr(command, "pastevents") == 0)
        {
            load_past_events(past_events, &count);
            if (count == 0 || strcmp(command, past_events[count - 1]) != 0)
            {
                if (count == MAX_COMMANDS)
                {
                    for (int i = 0; i < count - 1; i++)
                    {
                        strcpy(past_events[i], past_events[i + 1]);
                    }
                    count--;
                }
                strcpy(past_events[count], command);
                count++;
                save_past_events(past_events, count);
            }
        }
        if (strcmp(command, "exit") == 0)
        {
            break;
        }
        if (strncmp(command, "proclore", 8) == 0)
        {
            flag = 1;
            read_command(command);
        }
        if (strstr(command, ">") || strstr(command, "<"))
        {

            if (!strstr(command, "|"))
            {
                flag = 1;
                redirection(command);
            }
        }
        if (strstr(command, "|") && !(strstr(command, ">") || strstr(command, "<")))
        {
            flag = 1;
            execute_pipe(command);
        }
        if (strstr(command, "|") && (strstr(command, ">") || strstr(command, "<")))
        {
            flag = 1;
            execute_pipe_dir(command);
        }
        if (strcmp(command, "activities") == 0)
        {
            flag = 1;
            printActivities(bgProcesses, numBgProcesses);
        }
        if (command[strlen(command) - 2] == '&')
        {
            flag = 1;
            printf("hi\n");
            executeInBackground(command);
        }
        if (strncmp(command, "ping", 4) == 0)
        {
            flag = 1;
            handle_ping(command);
        }
        if (strncmp(command, "fg", 2) == 0)
        {
            flag = 1;
            handle_fg(command);
        }
        if (strncmp(command, "bg", 2) == 0)
        {
            flag = 1;
            handle_bg(command);
        }
        if (strncmp(command, "neonate -n ", 11) == 0)
        {
            flag = 1;
            int time_arg = atoi(command + 11);
            if (time_arg <= 0)
            {
                fprintf(stderr, "Invalid time_arg. Please provide a positive integer.\n");
                return 1;
            }

            // Register the handle_signal function to handle Ctrl+C
            signal(SIGINT, handle_signal);

            handle_neonate(time_arg);
        }
        if (strncmp(command, "iMan", 4) == 0)
        {
            flag = 1;
            // Extract the command name from the string
            char *command_name = command + 5;

            // Check if the command name is not empty
            if (*command_name != '\0')
            {
                fetch_man_page(command_name);
            }
            else
            {
                fprintf(stderr, "Usage: iMan <command_name>\n");
                return 1;
            }
        }

        if (flag == 0)
        {
            parse_and_execute(command, &count, cwd, home_dir, var, prev_dir, past_events);
        }
        flag = 0;
        identifier = 0;
    }
    while (numBgProcesses > 0)
    {
        handleBackgroundProcessTermination();
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    return 0;
}
