/* Name: 16.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to perform mandatory locking
 		      
 		      a. Implement write lock
		      b. Implement read lock
 
 * Date: 22 Aug 2024
*/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>

void apply_write_lock(int fd)
{
    struct flock lock;

    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("Failed to apply write lock");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Write lock applied.\n");
    }
}

void apply_read_lock(int fd)
{
    struct flock lock;

    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("Failed to apply read lock");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Read lock applied.\n");
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Please provide fine name and the mode(0 for read lock, 1 for write lock)>\n");
        exit(EXIT_FAILURE);
    }

    char* file = argv[1];
    char* mode = argv[2];

    int fd = open(file, O_RDWR | O_CREAT, 0664);
    if (fd == -1)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    if (fchmod(fd, 0664 | S_ISGID) == -1)  
    {
        perror("Failed to set file permissions for mandatory locking");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (strcmp(mode, "0") == 0)
    {
        apply_read_lock(fd);
    }
    else if (strcmp(mode, "1") == 0)
    {
        apply_write_lock(fd);
    }
    else
    {
        printf("You entered an invalid mode. Use '0' for read lock or '1' for write lock.\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Press Enter to release the lock and exit...\n");
    getchar();

    close(fd);

    return 0;
}

/*

To get the required output, I have created a text file namely 16.txt

To get the requred output, I opened two terminals and took various cases for applying the read and write locks

Command terminal 1: ./16
Output terminal 1: 
Please provide fine name and the mode(0 for read lock, 1 for write lock)>

Command terminal 1: ./16 16.txt 1					                Command terminal 2: ./16 16.txt 1
													
Output terminal 1:									Output terminal 2:
Write lock applied.									Failed to apply write lock: Resurce temporarily unavailable
Press Enter to release the lock and exit...			

Command terminal 1: ./16 16.txt 0					                Command terminal 2: ./16 16.txt 0
													
Output terminal 1:									Output terminal 2:
Read lock applied.									Read lock applied. 
Press Enter to release the lock and exit...			                        Press Enter to release the lock and exit... 

Command terminal 1: ./16 16.txt 1					                Command terminal 2: ./16 16.txt 0
													
Output terminal 1:									Output terminal 2:
Write lock applied.									Failed to apply read lock: Resorce temporarily unavailable 
Press Enter to release the lock and exit...			 

Command terminal 1: ./16 16.txt 0					                Command terminal 2: ./16 16.txt 1
													
Output terminal 1:									Output terminal 2:
Read lock applied.									Failed to apply write lock: Resorce temporarily unavailable
Press Enter to release the lock and exit...	 

*/

