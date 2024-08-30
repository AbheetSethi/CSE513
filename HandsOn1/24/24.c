/* Name: 24.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to create an orphan process
 * Date: 28 Aug 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid = fork();

    if (pid < 0) 
    {
        perror("Fork failed");
        return 1;
    } 
    
    else if (pid == 0) 
    {
        sleep(5);  // Make the child process sleep for a while
        printf("Child process (PID: %d) is now orphaned and adopted by init/systemd.\n", getpid());
        printf("Parent PID: %d\n", getppid()); // Print the new parent PID (which should be 1 or the PID of systemd)
    } 
    
    else 
    {
        printf("Parent process (PID: %d) is exiting.\n", getpid());
        exit(0);  // Parent exits immediately
    }

    return 0;
}

/*

Command: ./24

Output:

Parent process (PID: 12960) is exiting.

You'll first see the parent process message indicating that it has exited.
After a delay (due to the sleep(5) in the child), the child process will print its message, showing that its parent PID has changed to 1 or the PID of systemd.

abheet@abheet-OMEN-Laptop-15-ek0xxx:~/Desktop/HandsOn1/24$ Child process (PID: 12961) is now orphaned and adopted by init/systemd.
Parent PID: 1822

*/
