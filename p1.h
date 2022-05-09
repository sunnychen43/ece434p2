#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

#define test_string "Hello World"
#define NUM_TEAMS 4
#define SLEEP_TIME 30

void *team1work(void *arg);
void *team2work(void *arg);
void *team3work(void *arg);
void *team4work(void *arg);
void sig_handler(int sig);
void sig_handler_t1(int sig);
void sig_handler_t2(int sig);
void sig_handler_t3(int sig);
void sig_handler_t4(int sig);
