/* Name: 7.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to copy file1 into file2

 * Date: 8 Aug 2024
*/

#include<stdio.h>
#include<fcntl.h> // it contains open()
#include<unistd.h> // it contains, read, write, close
#define BUFFER_SIZE 1024

int main()
{
	
	int source_fd, dest_fd;
	int bytesRead, bytesWritten;
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
		bytesWritten = write(dest_fd, buffer, bytesRead);
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

/*

Command: cat 7_file1.txt

Output:
This is program number 7

Command: cat 7_file2.txt

Output:
**No Output**

Command: ./7

Output:
File copied successfully

Command: cat 7_file1.txt

Output:
This is program number 7

Command: cat 7_file2.txt

Output:
This is program number 7

*/
