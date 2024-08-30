/* Name: 6.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to take input from STDIN and display on STDOUT.
 * 		      Use only read/write system calls

 * Date: 8 Aug 2024
*/

#include<unistd.h>
#include<stdio.h>
#define BUFFER_SIZE 1024

int main()
{
	char buffer[BUFFER_SIZE];
	int bytesRead;

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

/*

Command: ./6

Input: 
This is program number 6

Output:
This is program number 6

*/
