# ifndef __INPUTS_H
# define __INPUTS_H
#define MAX_INPUT_SIZE 1024
#define MAX_COMMANDS 15
#define PAST_EVENTS_FILE "past_events.txt"
#define MAX_COMMAND_LENGTH 100
void execute_command(const char *command, int run_in_background, int *count, char *cwd, const char *home_dir, const char *var, char prev_dir[MAX_INPUT_SIZE], char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH]);
void parse_and_execute(const char *input, int *count, const char *cwd, const char *home_dir, const char *var, char prev_dir[MAX_INPUT_SIZE], char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH]);
# endif
