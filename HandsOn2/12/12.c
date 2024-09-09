/* Name: 12.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to create an orphan process. Use kill system call to send SIGKILL signal to the parent process from the child process.
 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

int main() 
{
    pid_t pid = fork(); // Create a new process (child)

    if (pid == -1) 
    {
        // Fork failed
        perror("Fork failed");
        exit(1);
    } 
    
    else if (pid == 0) 
    {
        // This is the child process
        printf("Child process: My PID is %d, my parent's PID is %d\n", getpid(), getppid());
        
        // Send SIGKILL signal to the parent process (getppid() gets parent PID)
        kill(getppid(), SIGKILL);

        // Sleep for a while to demonstrate orphan process behavior
        sleep(5);

        // After 5 seconds, the child's parent should be `init` or `systemd`
        printf("Child process: After killing parent, my new parent is PID %d\n", getppid());
    } 
    
    else 
    {
        // This is the parent process
        printf("Parent process: My PID is %d, my child's PID is %d\n", getpid(), pid);

        // Let the parent sleep for a while (so we can kill it from the child process)
        sleep(10);
    }

    return 0;
}

/*

Command: ./12

Output:

Parent process: My PID is 19098, my child's PID is 19099
Child process: My PID is 19099, my parent's PID is 19098
Killed
abheet@abheet-OMEN-Laptop-15-ek0xxx:~/Desktop/HandsOn2/12$ Child process: After killing parent, my new parent is PID 1785

*/



