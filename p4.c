#include "p4.h"

int main(int argc, char *argv[])
{

    // validate input params
    if (argc != NUM_TEAMS + 1)
    {
        fprintf(stderr, "usage: a.out <integer value> <integer value> <integer value> <integer value>\n");
        fprintf(stderr, "usage: each integer value is the number of threads you want for team 1,2,3, and 4 in order\n");
        exit(EXIT_FAILURE);
    }

    // load params into vars
    int numThreadsPerTeam[] = {atoi(argv[1]),
                               atoi(argv[2]),
                               atoi(argv[3]),
                               atoi(argv[4])};

    // validate input params for positve numbers
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        if (numThreadsPerTeam[i] <= 0)
        {
            fprintf(stderr, "usage: provided integer value for the number of threads in each team must be greater than 0\n");
            exit(EXIT_FAILURE);
        }
    }

    // declare thread vars
    pthread_t *tid[NUM_TEAMS];
    pthread_attr_t attr;

    // init thread ids
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        tid[i] = malloc(numThreadsPerTeam[i] * sizeof(pthread_t));
    }

    // create default attributes
    pthread_attr_init(&attr);

    // create array of function pointers to pass to threads
    void *(*func_ptr[4])(void *) = {team1work, team2work, team3work, team4work};

    // create all the threads by passing function pointers
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < numThreadsPerTeam[i]; j++)
        {
            pthread_create(&tid[i][j], &attr, func_ptr[i], NULL);
        }
    }

    // create masking variables
    sigset_t x;
    sigemptyset(&x);

    // add all signals to mask that main does not handle
    sigaddset(&x, SIGILL);
    sigaddset(&x, SIGFPE);
    sigaddset(&x, SIGHUP);
    sigaddset(&x, SIGSEGV);
    sigaddset(&x, SIGCHLD);
    // mask those signals
    pthread_sigmask(SIG_BLOCK, &x, NULL);

    // create signal handling variables
    struct sigaction sa;
    sa.sa_handler = &sig_handler_main;

    /*main thread handles SIGINT SIGABRT and SIGTSTP */
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    // send signals to threads
    sendAllSignals(tid[0][0]);

    // join all the threads
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < numThreadsPerTeam[i]; j++)
        {
            pthread_join(tid[i][j], NULL);
        }
    }

    // unblock the masked signals
    pthread_sigmask(SIG_UNBLOCK, &x, NULL);

    // return handling to default
    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    // notify the user that control has been restored
    puts("Control Restored to Main Thread");

    // wait for user to exit (SIGSINT or such command)
    pause();
    return EXIT_SUCCESS;
}

void sendAllSignals(pthread_t tid)
{
    pthread_kill(tid, SIGILL);
    pthread_kill(tid, SIGFPE);
    pthread_kill(tid, SIGHUP);
    pthread_kill(tid, SIGABRT);
    pthread_kill(tid, SIGCHLD);
    pthread_kill(tid, SIGINT);
    pthread_kill(tid, SIGSEGV);
    pthread_kill(tid, SIGTSTP);
}
void *team1work(void *param)
{
    // note all threads follow the same pattern, so only the first is commented

    // create masking variables
    sigset_t x;
    sigemptyset(&x);
    // add all signals to mask that the thread does not handle
    sigaddset(&x, SIGILL);
    sigaddset(&x, SIGFPE);
    sigaddset(&x, SIGHUP);
    sigaddset(&x, SIGABRT);
    sigaddset(&x, SIGCHLD);
    // mask those signals
    pthread_sigmask(SIG_BLOCK, &x, NULL);

    // create signal handling variables
    struct sigaction sa;
    sa.sa_handler = &sig_handler_t1;

    /*team 1 handles SIGINT SIGSEGV and SIGTSTP */
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    // wait some time for user to send signals
    sleep(SLEEP_TIME);

    // return
    return NULL;
}
void *team2work(void *param)
{
    sigset_t x;
    sigemptyset(&x);

    /*team 2 handles SIGINT SIGABRT and SIGCHLD */
    sigaddset(&x, SIGILL);
    sigaddset(&x, SIGFPE);
    sigaddset(&x, SIGHUP);
    sigaddset(&x, SIGTSTP);
    sigaddset(&x, SIGSEGV);

    pthread_sigmask(SIG_BLOCK, &x, NULL);

    struct sigaction sa;
    sa.sa_handler = &sig_handler_t2;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);

    sleep(SLEEP_TIME);

    return NULL;
}
void *team3work(void *param)
{

    sigset_t x;
    sigemptyset(&x);

    /*team 3 handles SIGTSTP SIGHUP and SIGFPE */
    sigaddset(&x, SIGINT);
    sigaddset(&x, SIGABRT);
    sigaddset(&x, SIGILL);
    sigaddset(&x, SIGCHLD);
    sigaddset(&x, SIGSEGV);

    pthread_sigmask(SIG_BLOCK, &x, NULL);

    struct sigaction sa;
    sa.sa_handler = &sig_handler_t3;

    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sleep(SLEEP_TIME);

    return NULL;
}
void *team4work(void *param)
{
    sigset_t x;
    sigemptyset(&x);

    /*team 4 handles SIGILL SIGCHLD and SIGSEGV*/
    sigaddset(&x, SIGINT);
    sigaddset(&x, SIGABRT);
    sigaddset(&x, SIGFPE);
    sigaddset(&x, SIGHUP);
    sigaddset(&x, SIGTSTP);

    pthread_sigmask(SIG_BLOCK, &x, NULL);

    struct sigaction sa;
    sa.sa_handler = &sig_handler_t4;

    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);

    sleep(SLEEP_TIME);

    return NULL;
}

void sig_handler_main(int sig)
{
    printf("Main Thread Catching ");
    sig_handler(sig);
}

void sig_handler_t1(int sig)
{
    printf("Team 1 Thread Catching ");
    sig_handler(sig);
}
void sig_handler_t2(int sig)
{
    printf("Team 2 Thread Catching ");
    sig_handler(sig);
}
void sig_handler_t3(int sig)
{
    printf("Team 3 Thread Catching ");
    sig_handler(sig);
}
void sig_handler_t4(int sig)
{
    printf("Team 4 Thread Catching ");
    sig_handler(sig);
}
void sig_handler(int sig)
{
    printf("Signal %s Caught by Thread Number: %ld\n", strsignal(sig), pthread_self());
}
