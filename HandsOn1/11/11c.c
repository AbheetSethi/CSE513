/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to open a file, duplicate the file descriptor and append the
  		      file with both the descriptor and check whether the file is updated properly
  		      or not
 
  		      C. Use fcntl
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int fd, fd_fcntl;
	const char* data1 = "Appending with original FD \n";
	const char* data2 = "Appending with duplicated FD (fcntl) \n";

	fd = open("11c.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	fd_fcntl = fcntl(fd, F_DUPFD, 0);
	if(fd_fcntl == -1)
	{
		perror("Error duplicating file descriptor using fcntl");
		close(fd);
		return 1;
	}

	if(write(fd, data1, strlen(data1)) == -1)
	{
		perror("Error writing to file with original FD");
		close(fd);
		close(fd_fcntl);
		return 1;
	}

	if(write(fd_fcntl, data2, strlen(data2)) == -1)
	{
		perror("Error writing to file with duplicated FD (fcntl)");
		close(fd);
		close(fd_fcntl);
		return 1;
	}

	close(fd);
	close(fd_fcntl);

	printf("File updated successfully with fcntl \n");
	return 0;
}
			
