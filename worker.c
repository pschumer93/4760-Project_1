#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

typedef struct
{
   int seconds;
   int nanoseconds;
}
Clock;

int main(int argc, char**argv)
{
   key_t key = ftok(".", 's');
   int shmid = shmget(key, SHM_SIZE, 0666);
   if (shmid == -1)
   {
      perror("shmget");
      exit(EXIT_FAILURE);
   }
   Clock *clock = (Clock *)shmat(shmid, NULL, 0);
   if(clock == (Clock *)(-1))
   {
      perror("shmat");
      exit(EXIT_FAILURE);
   }
   //int secondsPassed = atoi(argv[1]);
   //int nanosecondsPassed = atoi(argv[2]);
   int secondsPassed = 5;
   int nanosecondsPassed = 50000;

   int seconds = clock->seconds;
   int nanoseconds = clock->nanoseconds;

   int terminationSeconds = secondsPassed + clock->seconds;
   int terminationNanoseconds = nanosecondsPassed + clock->nanoseconds;
   int totalTermination = terminationSeconds + terminationNanoseconds;
    
   int i;
   printf("Worker PID: %d PPID: %d SysClockSec: %d SysClockNano: %d ", getpid(), getppid(),  clock->seconds, clock->nanoseconds);
   printf("TermTimeS: %d, TermTimeNano: %d\n", terminationSeconds, terminationNanoseconds);
   printf("--Just Starting\n\n");
   sleep(1);
   clock->seconds++;
   for(i = clock->seconds; i <= terminationSeconds; i++)
   {
      if(i == terminationSeconds)
      {
         printf("Worker PID: %d PPID: %d SysClockSec: %d SysClockNano: %d ", getpid(), getppid(),  clock->seconds, clock->nanoseconds);
         printf("TermTimeS: %d, TermTimeNano: %d\n", terminationSeconds, terminationNanoseconds);
         printf("--%d seconds have passed since starting\n\n",i );
         clock->seconds++;
         printf("Worker PID: %d PPID: %d SysClockSec: %d SysClockNano: %d ", getpid(), getppid(),  clock->seconds, clock->nanoseconds);
         printf("TermTimeS: %d, TermTimeNano: %d\n", terminationSeconds, terminationNanoseconds);
         printf("--Terminating\n");
        }
        if(i >= 1 && i < terminationSeconds)
        {
            printf("Worker PID: %d PPID: %d SysClockSec: %d SysClockNano: %d ", getpid(), getppid(),  clock->seconds, clock->nanoseconds);
            printf("TermTimeS: %d, TermTimeNano: %d\n", terminationSeconds, terminationNanoseconds);
            printf("--%d seconds have passed since starting\n\n",i );
            sleep(1);
            clock->seconds++;
        }
   }
   shmdt(clock);
   return EXIT_SUCCESS;
}
