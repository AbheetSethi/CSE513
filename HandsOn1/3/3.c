/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to create a file and print the file descriptor value. Use creat() system call.
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	const char *filename = "3.txt";

	int permissions = S_IRUSR | S_IWUSR;

	int fd = creat(filename, permissions);

	if(fd == -1)
	{
		perror("Error creating file");
		return 1;
	}

	printf("File '%s' created successfully. File descriptor: %d\n", filename, fd);

	close(fd);

	return 0;
}
