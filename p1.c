#include "p1.h"

int main(int argc, char *argv[])
{

    pthread_t *tid[NUM_TEAMS];
    pthread_attr_t attr;

    if (argc != NUM_TEAMS + 1)
    {
        fprintf(stderr, "usage: a.out <integer value> <integer value> <integer value> <integer value>\n");
        fprintf(stderr, "usage: each integer value is the number of threads you want for team 1,2,3, and 4 in order\n");
        exit(EXIT_FAILURE);
    }

    int numThreadsPerTeam[] = {atoi(argv[1]),
                               atoi(argv[2]),
                               atoi(argv[3]),
                               atoi(argv[4])};

    for (int i = 0; i < NUM_TEAMS; i++)
    {
        if (numThreadsPerTeam[i] <= 0)
        {
            fprintf(stderr, "usage: provided integer value for the number of threads in each team must be greater than 0\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_TEAMS; i++)
    {
        tid[i] = malloc(numThreadsPerTeam[i] * sizeof(pthread_t));
    }

    pthread_attr_init(&attr);

    // create array of function pointers to pass to threads
    void *(*func_ptr[4])(void *) = {team1work, team2work, team3work, team4work};

    // create all the threads
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < numThreadsPerTeam[i]; j++)
        {
            pthread_create(&tid[i][j], &attr, func_ptr[i], NULL);
        }
    }

    sigset_t x;
    sigemptyset(&x);

    sigaddset(&x, SIGILL);
    sigaddset(&x, SIGFPE);
    sigaddset(&x, SIGHUP);
    sigaddset(&x, SIGCHLD);
    pthread_sigmask(SIG_BLOCK, &x, NULL);

    struct sigaction sa;
    sa.sa_handler = &sig_handler_main;
    /*main thread handles SIGINT SIGABRT and SIGTSTP */
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    // join all the threads
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < numThreadsPerTeam[i]; j++)
        {
            pthread_join(tid[i][j], NULL);
        }
    }

    pthread_sigmask(SIG_UNBLOCK, &x, NULL);
    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    puts("Control Restored to Main Thread");
    pause();
    return EXIT_SUCCESS;
}

void *team1work(void *param)
{

    sigset_t x;
    sigemptyset(&x);
    /*team 1 handles SIGINT SIGSEGV and SIGTSTP */
    sigaddset(&x, SIGILL);
    sigaddset(&x, SIGFPE);
    sigaddset(&x, SIGHUP);
    sigaddset(&x, SIGABRT);
    sigaddset(&x, SIGCHLD);

    pthread_sigmask(SIG_BLOCK, &x, NULL);

    struct sigaction sa;
    sa.sa_handler = &sig_handler_t1;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    sleep(SLEEP_TIME);

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

/*pthread exit so each thread only catches the signal and reports its number once*/

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
    // pthread_exit(0);
}
