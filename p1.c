#include "p1.h"

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;

    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGSTOP, &sa, NULL);

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

    // join all the threads
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < numThreadsPerTeam[i]; j++)
        {
            pthread_join(tid[i][j], NULL);
        }
    }

    return EXIT_SUCCESS;
}

void *team1work(void *param)
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;

    /*team 1 handles SIGINT SIGSEGV and SIGSTOP */
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);

    sa.sa_handler = &sig_handler;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGSTOP, &sa, NULL);

    pause();
    sleep(SLEEP_TIME);

    return NULL;
}
void *team2work(void *param)
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;

    /*team 2 handles SIGINT SIGABRT and SIGCHLD */
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGSTOP, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);

    sa.sa_handler = &sig_handler;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);

    pause();
    sleep(SLEEP_TIME);

    return NULL;
}
void *team3work(void *param)
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;

    /*team 3 handles SIGSTOp SIGHUP and SIGFPE */
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);

    sa.sa_handler = &sig_handler;

    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGSTOP, &sa, NULL);
    pause();
    sleep(SLEEP_TIME);

    return NULL;
}
void *team4work(void *param)
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;

    /*team 4 handles SIGILL SIGCHLD and SIGSEGV*/
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGSTOP, &sa, NULL);

    sa.sa_handler = &sig_handler;

    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);

    pause();
    sleep(SLEEP_TIME);

    return NULL;
}

/*pthread exit so each thread only catches the signal and reports its number once*/
void sig_handler(int sig)
{
    printf("Signal Number %d Caught by Thread Number: %ld\n", sig, pthread_self());
    pthread_exit(0);
}
