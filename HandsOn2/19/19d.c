/* Name: 19d.c
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
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() 
{
    int status;

    // Create a FIFO using mknod system call
    status = mknod("19d_mknod", S_IFIFO | 0666, 0);

    if (status == -1) 
    {
        perror("mknod failed");
        return 1;
    }

    printf("FIFO created successfully using mknod!\n");

    return 0;
}

/*

Command: ./19d

Output:
FIFO created successfully using mknod!

*/
