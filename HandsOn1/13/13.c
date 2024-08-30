/* Name: 13.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to wait for a stdin for 10 seconds using select. Write a proper print
  		      statement to verify whether the data is available within 10 seconds or not.

 * Date: 22 Aug 2024
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> // contains STDIN_FILENO
#include<sys/select.h> // contains select() system call
#include<sys/time.h> // timeval structure definition is present here

int main()
{
	fd_set read_fds;
	struct timeval timeout; // this structure represents time interval
	int retval;

	FD_ZERO(&read_fds);  // This macro initializes the 'read_fds' set to empty, if we don't use it behaviour of the program will be unpredictable
	FD_SET(STDIN_FILENO, &read_fds); // This macro adds the file descriptor for standard input to the 'read_fds' set

	timeout.tv_sec = 10; // tv_sec is of long type 
	timeout.tv_usec = 0; // tv_usec is of long type

	retval = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);

	if(retval == -1)
	{
		perror("select()");
		exit(EXIT_FAILURE);
	}
	
	else if(retval == 0)
	{
		printf("No data within 10 seconds \n");
	}

	else
	{
		if(FD_ISSET(STDIN_FILENO, &read_fds))
		{
			char buffer[1024];
			int bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

			if(bytes_read > 0)
			{
				buffer[bytes_read] = '\0';
				printf("Data available: %s \n", buffer);
			}

			else
			{
				perror("read()");
			}
		}
	}

	return 0;
}

/*

Command: ./13

Output:
No data within 10 seconds

*/
