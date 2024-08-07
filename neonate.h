#ifndef __NEONATE_H_
#define __NEONATE_H_
void enable_raw_mode();
void disable_raw_mode();
void handle_signal(int signo);
void handle_neonate(int time_arg);
#endif