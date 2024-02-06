// Project_1
// Phillip Schumer
// CMP SCI 4760
// SPRING 2024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Process OSS
int main(int argc, char *argv[])
{
    /* Split each argv[?] string into individual characters.
        -The first charcter of argv[1] will be accessed like argv[1][0] = '?'.
        -The second character of argv[1] will be acessed like argv[1][1] = '?'.
        -Search if argc == 7, if not ERROR.
        -Search for appropriate character inputs, if not ERROR.
        -Search for -h, if so then HELP.
        -Only Appropriate inputs move on to fork. */
    if (argc != 7 || argv[1][0] != '-' || argv[1][1] != 'n' || argv[3][0] != '-' ||
        argv[3][1] != 's' || argv[5][0] != '-' || argv[5][1] != 't')
    {
        // If a input is -h, HELP.
        if(argv[1][1]== 'h')
        {
                printf("Help: oss\n");
                printf("---------\n");
                printf("FORMAT: ./oss -n proc -s simul -t iter\n\n");
                printf("\tproc = This parameter indicates the total number of user"
                " 'processes' you want oss to launch.\n\n");
                printf("\tsimul = This parameter indicates how many user processes"
                " oss are allowed to be launched 'simultaneously'.\n\n");
                printf("\titer = This parameter indicates the number 'iterations'"
                " to run inside each user process.\n\n");
                printf("EXAMPLE: ./oss -n 5 -s 3 -t 7\n");
                printf("\tThis command launches oss to run 5 total user procesess,"
                "3 processes will run at a time, and each user process will run 7 iterations.\n\n");
                exit(EXIT_SUCCESS);
        }

        // If input is not appropriate and does not include -h.
        else
        {
                printf("ERROR-Invalid Input.\n\tTry this format: %s -n proc -s simul -t iter\n\n", argv[0]);
                printf("\tFOR 'HELP' insert this command: %s -h\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    /* Only appropriate inputs make it to fork.
       Assign proc and simul. */
    int proc = atoi(argv[2]);
    int simul = atoi(argv[4]);

    /* If i is less than the total number processes, attempt to fork.
       Run a many user processes as simul will allow at a time.
       If simul is full, wait for current processes to end before running the rest.
       Run user processes simultaneously until i = proc. */
    for (int i = 0; i < proc; i += simul)
    {
        // If simul is not maxed out, run the fork.
        for (int j = 0; j < simul && i + j < proc; ++j)
        {
            // Fork the User Process.
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
                execl("./user", "user", argv[6], NULL);
                printf("Execution Failed\n");
                exit(EXIT_FAILURE);
            }
        }

        /* If simul is maxed out,
           wait to fork until other processes have ended before running the rest. */
        for (int j = 0; j < simul && i + j < proc; ++j)
        {
            wait(NULL);
        }
    }

    return 0;
}
