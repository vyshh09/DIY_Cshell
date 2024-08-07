#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H
#define MAX_INPUT_SIZE 1024
#define MAX_COMMANDS 15
#define MAX_COMMAND_LENGTH 100
#define PAST_EVENTS_FILE "past_events.txt"
void load_past_events(char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH], int *count);
void save_past_events(char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH], int count);
void handle_pastevents_command(const char *command, char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH], int *count, const char *cwd, const char *home_dir, const char *var, char prev_dir[MAX_INPUT_SIZE]);
void print_events(char past_events[MAX_COMMANDS][MAX_COMMAND_LENGTH], int *count);
#endif