/* Name: 3.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to create a file and print the file descriptor value. Use creat() system call.

 * Date: 8 Aug 2024
*/

#include<stdio.h>
#include<fcntl.h> // it contains creat(), S_IRUSR, S_IWUSR 
#include<unistd.h> // it contains close

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Pass the file name please \n");
		return 1;
	}

	char *filename = argv[1];

	int permissions = S_IRUSR | S_IWUSR;  // Read and Write permission for the file owner
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


/*

Command: ./3

Output: 
Pass the file name please

Command: ./3 3.txt

Output:
File '3.txt' created successfully. File descriptor: 3

*/
