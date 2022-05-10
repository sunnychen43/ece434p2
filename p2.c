#include "p2.h"

pthread_mutex_t lock[4]; // mutex to control access to bounds for each worker
int t1Bounds[2];         // array holding bounds for each team
int t2Bounds[2];
int t3Bounds[2];
int t4Bounds[2];

int *oddNumberArray; // array holding the list of generated primes
int *isPrimeArray;   // array holding the list of which numbers are prime

int main(int argc, char *argv[])
{

    // validate input params
    if (argc != 2)
    {
        fprintf(stderr, "usage: a.out <integer value>\n");
        fprintf(stderr, "usage: each integer value is the number of values in the array\n");
        exit(EXIT_FAILURE);
    }

    // load params into var
    int numThreadsPerTeam[] = {1, 10, 100, 1000};

    int num_elements = atoi(argv[1]);

    // validate input param for positve number
    if (num_elements <= 0)
    {
        fprintf(stderr, "usage: provided integer value for the number of elements in the array must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    // create random array and prime array
    oddNumberArray = malloc(num_elements * sizeof(int));
    isPrimeArray = malloc(num_elements * sizeof(int));

    // open the input file
    FILE *f = fopen("oddnumbers.txt", "wb");

    // populate array and file
    int val;
    for (int i = 0; i < num_elements; i++)
    {
        val = (rand() % ((MAX - MIN) / 2)) * 2 + 1 + MIN;
        oddNumberArray[i] = val;
        fprintf(f, "%d\n", val);
    }

    // close file
    fclose(f);

    // create subarray boundaries

    t1Bounds[0] = 0;
    t1Bounds[1] = t1Bounds[0] + num_elements / 4 - 1;
    t2Bounds[0] = t1Bounds[1] + 1;
    t2Bounds[1] = t2Bounds[0] + num_elements / 4 - 1;
    t3Bounds[0] = t2Bounds[1] + 1;
    t3Bounds[1] = t3Bounds[0] + num_elements / 4 - 1;
    t4Bounds[0] = t3Bounds[1] + 1;
    t4Bounds[1] = t4Bounds[0] + num_elements / 4 - 1;
    // declare thread vars
    pthread_t *tid[NUM_TEAMS];
    pthread_attr_t attr;

    // init thread ids and mutexes
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        tid[i] = malloc(numThreadsPerTeam[i] * sizeof(pthread_t));
        pthread_mutex_init(&lock[i], NULL);
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

    // join all the threads
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < numThreadsPerTeam[i]; j++)
        {
            pthread_join(tid[i][j], NULL);
        }
    }

    f = fopen("isPrime.txt", "wb");
    for (int i = 0; i < num_elements; i++)
    {
        fprintf(f, "%d\n", isPrimeArray[i]);
    }

    fclose(f);

    free(isPrimeArray);
    free(oddNumberArray);
    free(tid[0]);
    free(tid[1]);
    free(tid[2]);
    free(tid[3]);

    return EXIT_SUCCESS;
}

void *team1work(void *param)
{

    int val;
    while (1)
    {
        // lock mutex and acquire target index
        pthread_mutex_lock(&lock[0]);
        val = t1Bounds[1];

        // if the index is out of range, release mutex and break
        if (val < t1Bounds[0])
        {
            pthread_mutex_unlock(&lock[0]);
            break;
        }
        // otherwise decrement the index for next thread
        t1Bounds[1]--;
        // release the mutex
        pthread_mutex_unlock(&lock[0]);
        // calculate the prime and submit it
        isPrimeArray[val] = isPrime(oddNumberArray[val]);
    }

    // return
    return NULL;
}
void *team2work(void *param)
{
    int val;
    while (1)
    {
        // lock mutex and acquire target index
        pthread_mutex_lock(&lock[1]);
        val = t2Bounds[1];

        // if the index is out of range, release mutex and break
        if (val < t2Bounds[0])
        {
            pthread_mutex_unlock(&lock[1]);
            break;
        }
        // otherwise decrement the index for next thread
        t2Bounds[1]--;
        // release the mutex
        pthread_mutex_unlock(&lock[1]);
        // calculate the prime and submit it
        isPrimeArray[val] = isPrime(oddNumberArray[val]);
    }
    return NULL;
}
void *team3work(void *param)
{
    int val;
    while (1)
    {
        // lock mutex and acquire target index
        pthread_mutex_lock(&lock[2]);
        val = t3Bounds[1];

        // if the index is out of range, release mutex and break
        if (val < t3Bounds[0])
        {
            pthread_mutex_unlock(&lock[2]);
            break;
        }
        // otherwise decrement the index for next thread
        t3Bounds[1]--;
        // release the mutex
        pthread_mutex_unlock(&lock[2]);
        // calculate the prime and submit it
        isPrimeArray[val] = isPrime(oddNumberArray[val]);
    }
    return NULL;
}
void *team4work(void *param)
{
    int val;
    while (1)
    {
        // lock mutex and acquire target index
        pthread_mutex_lock(&lock[3]);
        val = t4Bounds[1];

        // if the index is out of range, release mutex and break
        if (val < t4Bounds[0])
        {
            pthread_mutex_unlock(&lock[3]);
            break;
        }
        // otherwise decrement the index for next thread
        t4Bounds[1]--;
        // release the mutex
        pthread_mutex_unlock(&lock[3]);
        // calculate the prime and submit it
        isPrimeArray[val] = isPrime(oddNumberArray[val]);
    }
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

int isPrime(int n)
{
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
        {
            return 0;
        }
    }
    return 1;
}