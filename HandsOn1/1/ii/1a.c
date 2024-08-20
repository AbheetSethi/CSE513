/* Name: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Create the following types of files using system call

 a. Soft Link (symlink system call)
*/

#include<unistd.h>
#include<stdio.h>

int main(void)
{
	if(symlink("1a.txt", "softlink") == -1)
	{
		perror("symlink");
		return 1;
	}

	return 0;
}

