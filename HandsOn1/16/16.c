/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to perform mandatory locking.
                      a. Implement write lock
                      b. Implement read lock
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

	if(fcntl(fd, F_SETLK, &lock) == -1)
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

	if(fcntl(fd, F_SETLK, &lock) == -1)
	{
		perror("Failed to apply read lock");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Read lock applied.\n");
	}
}

int main()
{
	int fd;

	fd = open("16.txt", O_RDWR | O_CREAT, 0664);

	if(fd == -1)
	{
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	if(fchmod(fd, 02664) == -1)
	{
		perror("Failed to set file permissions for mandatory locking");
		close(fd);
		exit(EXIT_FAILURE);
	}

	//write lock:
	//apply_write_lock(fd);
	
	//read lock:
	apply_read_lock(fd);

	printf("Press enter to release the lock and exit...\n");
	getchar();

	close(fd);

	return 0;
}
