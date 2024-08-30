/* Name: 12.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to find out the opening mode of a file. Use fcntl

 * Date: 22 Aug 2024
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

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
		case 0:
            printf("The file has been opened with the flags : O_RDONLY\n");
            break;
            
		case 1:
            printf("The file has been opened with the flags : O_WRONLY\n");
            break;
        
		case 2:
            printf("The file has been opened with the flags : O_RDWR\n");
            break;

		default:
			printf("Unknown file opening mode \n");
			break;
	}
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Pass the file to be opened as the argument and opening mode \n");
		return 1;
	}
	
	int fd;
	int flags = 0;

	char* file = argv[1];
	char* mode = argv[2];

	if (strcmp(mode, "0") == 0)
    {
        flags = O_RDONLY;
    }
    else if (strcmp(mode, "1") == 0)
    {
        flags = O_WRONLY | O_CREAT;  
    }
    else if (strcmp(mode, "2") == 0)
    {
        flags = O_RDWR | O_CREAT; 
    }

	fd = open(file, flags, 0644);

	if(fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	check_open_mode(fd);

	close(fd);

	return 0;

}

/*

Command: ./12 12.txt 0

Output:
The file has been opened with the flags : O_RDONLY

Command: ./12 12.txt 1

Output:
The file has been opened with the flags : O_WRONLY

CommandL ./12 12.txt 2

Output:
The file has been opened with the flags : O_RDWR

*/
