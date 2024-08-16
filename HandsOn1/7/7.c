/* Name: Abheet Sethi
 * Registration No.: MT2024004
 *
 * Problem Statement: Write a program to copy file1 into file2
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#define BUFFER_SIZE 1024

int main()
{
	int source_fd, dest_fd;
	ssize_t bytesRead, bytesWritten;
	char buffer[BUFFER_SIZE];

	source_fd = open("7_file1.txt", O_RDONLY);

	if(source_fd == -1)
	{
		perror("Error opening source file");
		return 1;
	}

	dest_fd = open("7_file2.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	if(dest_fd == -1)
	{
		perror("Error opening/creating destination file");
		close(source_fd);
		return 1;
	}

	while((bytesRead = read(source_fd, buffer, BUFFER_SIZE)) > 0)
	{
		if (bytesWritten != bytesRead)
		{
			perror("Error writing to destination file");
			close(source_fd);
			close(dest_fd);
			return 1;
		}
	}

	if(bytesRead == -1)
	{
		perror("Error reading from source file");
	}

	close(source_fd);
	close(dest_fd);

	printf("File copied successfully. \n");

	return 0;
}
