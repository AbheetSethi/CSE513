/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Crete the following types of files using system call:
 
   b. Hard Link (link system call)
*/

#include<unistd.h>
#include<stdio.h>

int main(void)
{
	if(link("1b.txt", "1b") == -1)
	{
		perror("link");
		return 1;
	}
	return 0;
}

