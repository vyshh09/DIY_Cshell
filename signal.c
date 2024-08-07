# include "headers.h"
extern pid_t fgProcessPid;

extern struct termios original_termios;
void handleSIGINT(int signo)
{
    if (signo == SIGINT && fgProcessPid > 0)
    {
        kill(fgProcessPid, SIGINT);
    }
}

void restoreTerminalAttributes()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}