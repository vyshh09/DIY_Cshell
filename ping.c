# include "headers.h"

void handle_ping( char* command)
{
    int pid, signal_num;
            if (sscanf(command, "ping %d %d", &pid, &signal_num) == 2)
            {
              if (kill(pid, 0) == 0)
                {
                    int signal_to_send = signal_num % 32; // Modulo 32 to map to valid signal numbers
                    if (kill(pid, signal_to_send) == 0)
                    {
                        printf("Sent signal %d to process with PID %d\n", signal_to_send, pid);
                        sleep(1); // Wait for the process to respond to the signal
                        int status = printProcessStatus(pid);
                        if (status == -1)
                        {
                            printf("Failed to get process status\n");
                        }
                        else if (status == 0)
                        {
                            printf("Process with PID %d is still running\n", pid);
                        }
                        else
                        {
                            printf("Process with PID %d has exited\n", pid);
                        }
                    }
                    else
                    {
                        perror("kill");
                    }
                }
                else
                {
                    printf("No such process found with PID %d\n", pid);
                }
            }
            else
            {
                printf("Invalid syntax for 'ping' command\n");
            }
}