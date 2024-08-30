/* Name: 5.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to create five new files with infinite loop. 
                      Execute the program in the background and check the file descriptor table at /proc/pid/fd

 * Date: 8 Aug 2024
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

/*

Command: ./5 &

Output:
[1] 14060
abheet@abheet-OMEN-Laptop-15-ek0xxx:~/Desktop/HandsOn1/5$ File '5_1.txt' created successfully. File descriptor: 3 
File '5_2.txt' created successfully. File descriptor: 4 
File '5_3.txt' created successfully. File descriptor: 5 
File '5_4.txt' created successfully. File descriptor: 6 
File '5_5.txt' created successfully. File descriptor: 7 

Command: ls -l /proc/14060/fd
total 0
lrwx------ 1 abheet abheet 64 Aug 28 21:12 0 -> /dev/pts/0
lrwx------ 1 abheet abheet 64 Aug 28 21:12 1 -> /dev/pts/0
lrwx------ 1 abheet abheet 64 Aug 28 21:12 2 -> /dev/pts/0
l-wx------ 1 abheet abheet 64 Aug 28 21:12 3 -> /home/abheet/Desktop/HandsOn1/5/5_1.txt
l-wx------ 1 abheet abheet 64 Aug 28 21:12 4 -> /home/abheet/Desktop/HandsOn1/5/5_2.txt
l-wx------ 1 abheet abheet 64 Aug 28 21:12 5 -> /home/abheet/Desktop/HandsOn1/5/5_3.txt
l-wx------ 1 abheet abheet 64 Aug 28 21:12 6 -> /home/abheet/Desktop/HandsOn1/5/5_4.txt
l-wx------ 1 abheet abheet 64 Aug 28 21:12 7 -> /home/abheet/Desktop/HandsOn1/5/5_5.txt

*/

