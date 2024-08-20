/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to wait for a stdin for 10 seconds using select. Write a proper print
  		      statement to verify whether the data is available within 10 seconds or not.
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/select.h>
#include<sys/time.h>

int main()
{
	fd_set read_fds;
	struct timeval timeout;
	int retval;

	FD_ZERO(&read_fds);
	FD_SET(STDIN_FILENO, &read_fds);

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

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
			ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

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
