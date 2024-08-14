/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to create five new files with infinite loop. 
  		      Execute the program in the background and check the file descriptor table at /proc/pid/fd
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	int fds[5], i;
	const char *filenames[5] = {"5_1.txt", "5_2.txt", "5_3.txt", "5_4.txt", "5_5.txt"};

	for(i=0; i<5; i++)
	{
		fds[i] = creat(filenames[i], S_IRUSR | S_IWUSR);

		if(fds[i] == -1)
		{
			perror("Error creating file");
			return 1;
		}

		printf("File '%s' created successfully. File descriptor: %d \n", filenames[i], fds[i]);
	}

	while(1)
	{
		sleep(1);
	}

	for(i=0; i<5; i++)
	{
		close(fds[i]);
	}

	return 0;
}


