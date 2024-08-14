/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to open an existing file with read write mode. Try O_EXCL flag also.
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	const char *filename = "4.txt";

	int fd = open(filename, O_RDWR | O_EXCL);

	if(fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	printf("File '%s' opened successfully. File descriptor: %d\n", filename, fd);

	close(fd);

	return 0;
}
