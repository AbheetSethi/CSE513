/* Name: 19e.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Create a FIFO file by
                        a. mknod command
                        b. mkfifo command
                        c. use strace command to find out, which command (mknod or mkfifo) is better.
                        d. mknod system call
                        e. mkfifo library function
 * Date: 12 Sept 2024
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main() 
{
    int status;

    // Create a FIFO using mkfifo library function
    status = mkfifo("19e_mkfifo", 0666);

    if (status == -1) 
    {
        perror("mkfifo failed");
        return 1;
    }

    printf("FIFO created successfully using mkfifo!\n");

    return 0;
}

/*

Command: ./19e

Output:
FIFO created successfully using mkfifo!

*/