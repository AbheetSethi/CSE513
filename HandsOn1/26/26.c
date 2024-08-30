/* Name: 26.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to execute an executable program.
 
  		      a. use some executable program
  		      b. pass some input to an executable program. (for example execute an executable of ./a.out name)
 */


#include <unistd.h>    // Import for `execl`
#include <sys/types.h> // Import for `fork`
#include <sys/wait.h>  // Import for `waitpid`
#include <stdio.h>

int main(int argc, char *argv[])
{
    
    if (argc != 2)
    {
        printf("Kindly enter the input \n");
        return 1;
    }
    
    int pid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        printf("Executing ./a.out with argument: %s\n", argv[1]);

        // Replace the current process image with the executable
        execl("./26_executable", "./26_executable", argv[1], NULL);

        // If execl returns, it means there was an error
        perror("execl failed");
        return 1;
    }

    else if (pid > 0)
    {
        // Parent process waits for the child to complete
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        else
        {
            printf("Child process did not terminate normally.\n");
        }
    }

    else
    {
        perror("fork failed");
        return 1;
    }

    return 0;
}

/*

Command: ./26 MT2024004

Output:
Executing ./a.out with argument: MT2024004
Your roll number is: MT2024004!
Child exited with status 0

*/
