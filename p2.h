#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

#define NUM_TEAMS 4   // number of teams
#define SLEEP_TIME 30 // time to sleep before joining threads

/**
 * @brief
 * thread function, handles
 * SIGINT
 * SIGSEGV
 * SIGTSTP
 * using sig_handlert1
 * @param void
 * @return void
 */
void *team1work(void *arg);
/**
 * @brief
 * thread function, handles
 * SIGINT
 * SIGABRT
 * SIGCHLD
 * using sig_handlert2
 * @param void
 * @return void
 */
void *team2work(void *arg);
/**
 * @brief
 * thread function, handles
 * SIGTSTP
 * SIGHUP
 * SIGFPE
 * using sig_handlert3
 * @param void
 * @return void
 */
void *team3work(void *arg);
/**
 * @brief
 * thread function, handles
 * SIGILL
 * SIGCHLD
 * SIGSEGV
 * using sig_handlert4
 * @param void
 * @return void
 */
void *team4work(void *arg);

/**
 * @brief
 * base handler, prints the name of the signal
 * and the thread id
 * @param sig
 * the signal caught
 * @return void
 */
void sig_handler(int sig);
/**
 * @brief
 * main handler, prefixes the message with "Main"
 * @param sig
 * the signal caught
 * @return void
 */
void sig_handler_main(int sig);
/**
 * @brief
 * Team 1 handler, prefixes the message with "Team 1"
 * @param sig
 * the signal caught
 * @return void
 */
void sig_handler_t1(int sig);
/**
 * @brief
 * Team 2 handler, prefixes the message with "Team 2"
 * @param sig
 * the signal caught
 * @return void
 */
void sig_handler_t2(int sig);
/**
 * @brief
 * Team 3 handler, prefixes the message with "Team 3"
 * @param sig
 * the signal caught
 * @return void
 */
void sig_handler_t3(int sig);
/**
 * @brief
 * Team 4 handler, prefixes the message with "Team 4"
 * @param sig
 * the signal caught
 * @return void
 */
void sig_handler_t4(int sig);
