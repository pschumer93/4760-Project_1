// Project_1
// Phillip Schumer
// CMP SCI 4760
// Spring 2024

#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>

//Process OSS
int main(int argc, char** argv)
{
                /* -size = the number of iterations passed to user in argv[1].
                   -argv[1] will be passed as a string.
                   -argv[1] must be convertd to int.*/

                int size = atoi(argv[1]);
                for(int i = 1; i <= size; i++)
                {
                        // Print the PID and PPID before sleeping
                        printf("USER PID: %d ",getpid());
                        printf("USER PPID: %d ",getppid());
                        /* NOTE: If called directly like "./user 5" the PPID will be random,
                           because the fork which is located in oss. */
                        printf("Iteration: %d before sleeping\n", i);

                        // Sleep for 1 second
                        sleep(1);

                        // Print the PID and PPID after sleeping.
                        printf("USER PID: %d ",getpid());
                        printf("USER PPID: %d ",getppid());
                        /* NOTE: If called directly like "./user 5" the PPID will be random,
                           because the fork which is located in oss. */
                        printf("Iteration: %d after sleeping\n", i);
                }

                return EXIT_SUCCESS;
}
