/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to find out the opening mode of a file. Use fcntl
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

void check_open_mode(int fd)
{
	int flags = fcntl(fd, F_GETFL);
	if(flags == -1)
	{
		perror("Error getting file flags");
		return;
	}

	int access_mode = flags & O_ACCMODE;

	switch(access_mode)
	{
		case O_RDONLY:
			printf("File is opened in read-only mode \n");
			break;

		case O_WRONLY:
			printf("File is opened in write-only mode \n");
			break;

		case O_RDWR:
			printf("File is opened in read-write mode \n");
			break;

		default:
			printf("Unknown file opening mode \n");
			break;
	}
}

int main()
{
	int fd;

	fd = open("12.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	if(fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	check_open_mode(fd);

	close(fd);

	return 0;

}
