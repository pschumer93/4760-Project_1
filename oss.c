#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

typedef struct
{
    int seconds;
    int nanoseconds;
}
Clock;

int incrementNano( int nano)
{
    nano += 500000000; // Increment nanoseconds by 500 million (half second)
    return nano;
}

int incrementSec(int nano, int sec)
{
    if (nano >= 1000000000)
    {
         sec++;
    }
    usleep(500000);
    return sec;
}

int decrementNano(int nano)
{
        if (nano >= 1000000000)
        {
               nano  -= 1000000000;
        }
        return nano;
}

int main(int argc, char** argv)
{
    key_t key = ftok(".", 's');
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    Clock *clock = (Clock *)shmat(shmid, NULL, 0);
    if (clock == (Clock *)(-1))
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    clock->seconds = 0;
    clock->nanoseconds=0;
    printf("Initialized shared data with seconds = %d and nanoseconds = %d\n", clock->seconds, clock->nanoseconds);
    int i;
    int nanoseconds = 0;
    int seconds = 0;
    for(i = 0; i < 1 ; i++)
    {
        nanoseconds=incrementNano(nanoseconds);
        seconds=incrementSec(nanoseconds, seconds);
        nanoseconds=decrementNano(nanoseconds);
        clock->nanoseconds = nanoseconds;
        clock->seconds = seconds;

         pid_t pid = fork();
        // If fork failure.
        if (pid == -1)
        {
                printf("Fork Failed\n");
                exit(EXIT_FAILURE);
        }
        // If fork sucess.
        if (pid == 0)
        {
                // Call User Process with iter (argv[6]).
                execl("./worker", "woker", NULL);
                printf("Execution Failed\n");
                exit(EXIT_FAILURE);
        }
        wait(NULL);
    }
    printf("Were Done\n");
    shmdt(clock);
    return 0;
}
                          