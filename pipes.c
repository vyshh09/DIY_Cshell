#include "headers.h"
void execute_command_pipe(char *command)
{
    char *args[MAX_COMMANDS];
    int arg_count = 0;

    char *token = strtok(command, " ");
    while (token != NULL)
    {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    execvp(args[0], args);
    perror("execvp");
    exit(EXIT_FAILURE);
}

void execute_pipe(char *input)
{
    char *commands[MAX_COMMANDS];
        int command_count = 0;
    char *token = strtok(input, "|");
    while (token != NULL)
    {
        commands[command_count++] = token;
        token = strtok(NULL, "|");
    }

    if (command_count < 2)
    {
        // Invalid use of pipe
        fprintf(stderr, "Invalid use of pipe\n");
        return;
    }

    int pipes[2];
    int input_fd = 0;

    for (int i = 0; i < command_count; i++)
    {
        pipe(pipes);

        if (fork() == 0)
        {
            // Child process
            close(pipes[0]); // Close the read end of the pipe

            // Redirect stdin to the input of the pipe (except for the first command)
            if (i != 0)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // Redirect stdout to the output of the pipe (except for the last command)
            if (i != command_count - 1)
            {
                dup2(pipes[1], STDOUT_FILENO);
                close(pipes[1]);
            }

            execute_command_pipe(commands[i]);
        }
        else
        {
            // Parent process
            close(pipes[1]); // Close the write end of the pipe
            wait(NULL);
            input_fd = pipes[0];
        }
    }
}
void execute_pipe_dir(char *input)
{
    char *commands[MAX_COMMANDS];
    int command_count = 0;
    char *token = strtok(input, "|");
    while (token != NULL)
    {
        commands[command_count++] = token;
        token = strtok(NULL, "|");
    }

    int pipes[2];
    int input_fd = 0;

    for (int i = 0; i < command_count; i++)
    {
        pipe(pipes);

        if (fork() == 0)
        {
            // Child process
            close(pipes[0]); // Close the read end of the pipe
            // Redirect stdin to the input of the pipe (except for the first command)
            if (i != 0)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // Handle output redirection for the last command
            if (i == command_count - 1)
            {
                char *redir_token = strtok(commands[i], ">");
                if (redir_token != NULL)
                {
                    char *file_name = strtok(NULL, " \n");
                    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                    if (fd == -1)
                    {
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }
            else
            {
                // Redirect stdout to the output of the pipe (except for the last command)
                dup2(pipes[1], STDOUT_FILENO);
                close(pipes[1]);
            }

            execute_command_pipe(commands[i]);
        }
        else
        {
            // Parent process
            close(pipes[1]); // Close the write end of the pipe
            wait(NULL);
            input_fd = pipes[0];
        }
    }
}

