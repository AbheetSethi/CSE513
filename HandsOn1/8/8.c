/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to open a file in read only mode read line by line and display each line
 * 		      as it is read. Close the file when end of file is reached
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

#define BUFFER_SIZE 256

int main()
{
	int fd, i, start;
	ssize_t bytesRead;
	char buffer[BUFFER_SIZE];
	char *line;

	fd = open("8.txt", O_RDONLY);
	if(fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	while((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		start = 0;

		for(i=0; i < bytesRead; i++)
		{
			if(buffer[i] == '\n')
			{
				buffer[i] = '\0';
				printf("%s\n", &buffer[start]);
				start = i+1;
			}
		}

		if(start < bytesRead)
		{
			printf("%s", &buffer[start]);
		}
	}

	if(bytesRead == -1)
	{
		perror("Error reading file");
		close(fd);
		return 1;
	}

	close(fd);

	return 0;
}
