# ifndef __FG_BG_H_
# define __FG_BG_H_
void handle_bg(char *command);
void handle_fg(char *command);
int resumeBackgroundProcess(pid_t pid);
int bringToForeground(pid_t pid);
# endif