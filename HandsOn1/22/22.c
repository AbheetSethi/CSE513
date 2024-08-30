/* Name: 22.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
  
 * Problem Statement: Write a program, open a file, call fork and then write to the file by both 
                      the child as well as the parent processes. Check output of the file.

 * Date: 28 Aug 2024
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h> 

int main() 
{
    
    int fd = open("22.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) 
    {
        perror("Failed to open file");
        return 1;
    }

    int pid = fork();

    if (pid < 0) 
    {
        perror("Fork failed");
        return 1;
    } 
    
    else if (pid == 0) 
    {
        const char *child_msg = "This is the child process writing.\n";
        write(fd, child_msg, strlen(child_msg));
        close(fd);
    } 
    
    else 
    {
        const char *parent_msg = "This is the parent process writing.\n";
        write(fd, parent_msg, strlen(parent_msg));

        wait(NULL);
        close(fd);
    }

    return 0;
}

/*

Command: ./22
         cat 22.txt

Output:

This is the parent process writing.
This is the child process writing.

*/
