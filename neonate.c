#include "headers.h"

volatile sig_atomic_t stop_flag = 0;

void enable_raw_mode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode()
{
    struct termios cooked;
    tcgetattr(STDIN_FILENO, &cooked);
    cooked.c_lflag |= (ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &cooked);
}

void handle_signal(int signo)
{
    if (signo == SIGINT)
    {
        // Handle SIGINT (Ctrl+C) by setting the stop flag
        stop_flag = 1;
    }
}
void handle_neonate(int time_arg)
{
    enable_raw_mode();

    pid_t last_pid = 0;

    while (!stop_flag)
    {
        // Set up a file descriptor set for select
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        // Use select to check for available input
        int ready = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);

        if (ready == -1)
        {
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            char ch;
            if (read(STDIN_FILENO, &ch, 1) == 1 && ch == 'x')
            {
                stop_flag = 1;
                break;
            }
        }

        pid_t current_pid = fork();

        if (current_pid == -1)
        {
            perror("fork");
            break;
        }
        else if (current_pid == 0)
        {
            // This is the child process
            printf("%d\n", getpid());
            exit(0);
        }
        else
        {
            // This is the parent process
            last_pid = current_pid;
        }

        sleep(time_arg);
    }

    if (last_pid != 0)
    {
        kill(last_pid, SIGTERM);
    }

    disable_raw_mode();
    stop_flag=0;
}
