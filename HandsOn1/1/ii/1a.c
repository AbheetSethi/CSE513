/* Name: 1a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Create the following types of files using system call

                      a. Soft Link (symlink system call)

 * Date: 8 Aug 2024
*/

#include<unistd.h> // contains link, symlink
#include<stdio.h>

int main(void)
{
	if(symlink("1a.txt", "softlink_1a") == -1)
	{
		perror("Error while creating Softlink");
		return 1;
	}

	else
	{
		printf("Soft Link Creation Successful ! \n");
		printf("Type ll to check \n");
	}

	return 0;
}


/* 
 Command: ./a
 
 Output:
  
 Soft Link Creation Successful !
 Type ll to check

 Command: ll
 
 Output:
 
 total 68
 drwxrwxr-x  2 abheet abheet 4096  Aug 28 18:37 ./
 drwxrwxr-x  4 abheet abheet 4096  Aug 8  22:57 ../
 -rwxrwxr-x  4 abheet abheet 16048 Aug 28 18:37 1a*
 -rw-rw-r--  1 abheet abheet   440 Aug 28 18:36 1a.txt
 lrwxrwxrwx 1 abheet abheet     6 Aug 28 18:37 softlink_1a -> 1a.txt
*/

