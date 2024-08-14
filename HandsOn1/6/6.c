/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to take input from STDIN and display on STDOUT.
 * 		      Use only read/write system calls
*/

#include<unistd.h>
#include<stdio.h>
#define BUFFER_SIZE 1024

int main()
{
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead;

	while((bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
	{
		write(STDIN_FILENO, buffer, bytesRead);
	}

	if(bytesRead == -1)
	{
		perror("Error reading from STDIN");
		return 1;
	}

	return 0;
}
