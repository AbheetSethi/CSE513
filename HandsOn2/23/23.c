/* Name: 23.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).
 * Date: 12 Sept 2024
*/

#include <unistd.h>    // Import for `sysconf`, `sysconf`
#include <sys/types.h> // Import for `mkfifo`
#include <sys/stat.h>  // Import for `mkfifo`
#include <stdio.h>     
#include <errno.h>     // Import for `errno`

void main()
{
    int fifoStatus;  // Determines success of `mkfifo` call
    long maxFiles;   // Maximum number of files that can be opened within process
    long sizeOfPipe; // The maximim size of a FIFO file

    char *fifoName = "23";
    fifoStatus = mkfifo(fifoName, S_IRWXU);

    if (!(errno == EEXIST || fifoStatus != -1))
        perror("Error while creating FIFO file!");
    else
    {
        maxFiles = sysconf(_SC_OPEN_MAX);

        if (maxFiles == -1)
            perror("Error while calling sysconf!");
        else
            printf("Maximum number of files that can be opened is: %ld\n", maxFiles);

        sizeOfPipe = pathconf(fifoName, _PC_PIPE_BUF);

        if (sizeOfPipe == -1)
            perror("Error while calling pathconf!");
        else
            printf("Maximum size of pipe: %ld\n", sizeOfPipe);
    }
}

/*

Command: ./23

Output:
Maximum number of files that can be opened is: 1024
Maximum size of pipe: 4096

*/