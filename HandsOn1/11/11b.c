/* Name: Abheet Sethi
 * Registration No. MT2024004
 
 * Problem Statement: Write a program to open a file, duplicate the file descriptor and append the file with
  		      both the descriptor and check whether the file is updated properly or not
 
  		      B. Use dup2
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int fd, fd_dup2;
	const char* data1 = "Appending with original FD \n";
	const char* data2 = "Appending with duplicated FD (dup2) \n";

	fd = open("11b.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	fd_dup2 = dup2(fd, 5);
	if(fd_dup2 == -1)
	{
		perror("Error duplicating file descriptor using dup2");
		close(fd);
		return 1;
	}

	if(write(fd, data1, strlen(data1)) == -1)
	{
		perror("Error writing to file with original FD");
		close(fd);
		close(fd_dup2);
		return 1;
	}

	if(write(fd_dup2, data2, strlen(data2)) == -1)
	{
		perror("Error writing to file with duplicated FD (dup2)");
		close(fd);
		close(fd_dup2);
		return 1;
	}

	close(fd);
	close(fd_dup2);

	printf("File updated successfully with dup2 \n");
	return 0;
}
