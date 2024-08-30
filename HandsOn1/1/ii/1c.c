/* Name: 1c.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Create the following types of files using system call:
 
                      c. FIFO (mkfifo Library Function or mknod system call)

 * Date: 8 Aug 2024
*/


#include<sys/types.h> // includes definitions for data types used in system calls
#include<sys/stat.h> // contains mknod, mkfifo
#include<stdio.h>

int main(void)
{
	if(mkfifo("1c.txt", 0666) == -1)
	{
		perror("Error while creating FIFO");
		return 1;
	}

	printf("Fifo file creation successful \n");
	printf("Type ll to check \n");
	
	return 0;
}


/*

Command: ./1c

Output:

Fifo file creation successful 
Type ll to check 

Command: ll

Output:

total 68
drwxrwxr-x 2 abheet abheet  4096 Aug 28 19:16 ./
drwxrwxr-x 4 abheet abheet  4096 Aug  8 22:57 ../
-rwxrwxr-x 1 abheet abheet 16040 Aug 28 19:16 1c*
-rw-rw-r-- 1 abheet abheet   479 Aug 28 19:15 1c.c
prw-rw-r-- 1 abheet abheet     0 Aug 28 19:16 1c.txt|

*/
