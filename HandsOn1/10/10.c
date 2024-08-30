/* Name: 10.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10 bytes (use lseek) and
  		      write again 10 bytes
 
  		      A. Check the return value of lseek
  		      B. Open the file with od and check the empty spaces in between the data

 * Date: 8 Aug 2024
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int fd;
	off_t offset;
	int bytes_written;
	const char *data1 = "0123456789";
	const char *data2 = "ABCDEFGHIJ";

	fd = open("10.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	if(fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	bytes_written = write(fd, data1, strlen(data1));
	if(bytes_written == -1)
	{
		perror("Error writing to file");
		close(fd);
		return 1;
	}

	printf("lseek moved the file pointer to offset: %ld \n", offset);

	bytes_written = write(fd, data2, strlen(data2));
	if(bytes_written == -1)
	{
		perror("Error writing to file");
		close(fd);
		return 1;
	}

	close(fd);

	printf("Data written successfully \n");
	return 0;
}

/*

Command: ./10

Output:
lseek moved the file pointer to offset: 139941193726640
Data written successfully

*/


