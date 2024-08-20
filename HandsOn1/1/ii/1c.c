/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Create the following types of files using system call:
 
                      c. FIFO (mkfifo Library Function or mknod system call)
*/


#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>

int main(void)
{
	if(mknod("1c.txt", S_IFIFO | 0666, 0) == -1)
	{
		perror("mknod");
		return 1;
	}
	return 0;
}
