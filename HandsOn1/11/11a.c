/* Name: 11a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to open a file, duplicate the file descriptor and append the file
  		      with both the descriptor and check whether the file is updated properly or not.
 
  		      A. Use dup

 * Date: 8 Aug 2024
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int fd, fd_dup;
	const char* data1 = "Appending with original FD \n";
	const char* data2 = "Appending with duplicated FD \n";

	fd = open("11a.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	fd_dup = dup(fd);
	if(fd_dup == -1)
	{
		perror("Error duplicating file descriptor");
		close(fd);
		return 1;
	}

	if(write(fd, data1, strlen(data1)) == -1)
	{
		perror("Error writing to file with original FD");
		close(fd);
		close(fd_dup);
		return 1;
	}

	if(write(fd_dup, data2, strlen(data2)) == -1)
	{
		perror("Error writing to file with duplicated FD");
		close(fd);
		close(fd_dup);
		return 1;
	}

	close(fd);
	close(fd_dup);

	printf("File updated successfully with dup. \n");
	return 0;
}

/*

Command: cat 11a.txt

Output:
Name: Abheet Sethi
Registration No.: MT2024004

This is program number 11a

Command: ./11a

Output:
File updated successfully with dup. 

Command: cat 11a.txt

Output:
Name: Abheet Sethi
Registration No.: MT2024004

This is program number 11a
Appending with original FD 
Appending with duplicated FD 

*/
