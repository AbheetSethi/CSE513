/* Name: 4.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to open an existing file with read write mode. Try O_EXCL flag also.

 * Date: 8 Aug 2024
*/

#include<stdio.h>
#include<fcntl.h> // O_RDWR, O_EXCL are included
#include<unistd.h> //it contains close()

int main()
{
	char *filename = "4.txt";

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

/*

Command: ./4

Output:
File '4.txt' opened successfully. File descriptor: 3

*/
