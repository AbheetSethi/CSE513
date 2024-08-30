/* Name: 1b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Crete the following types of files using system call:
 
                      b. Hard Link (link system call)

 * Date: 8 Aug 2024
*/

#include<unistd.h> // contains link, symlink
#include<stdio.h>

int main(void)
{
	if(link("1b.txt", "hardlink_1b") == -1)
	{
		perror("Error while creating Hardlink");
		return 1;
	}
	
	else
	{
		printf("Hard Link Creation Successful ! \n");
		printf("Type ll to check \n");
	}

	return 0;
}

/*

Command: ./1b

Output:
Hard Link Creation Successful ! 
Type ll to check 

Command: ll

Output:
total 68
drwxrwxr-x 2 abheet abheet  4096 Aug 28 18:53 ./
drwxrwxr-x 4 abheet abheet  4096 Aug  8 22:57 ../
-rwxrwxr-x 1 abheet abheet 16040 Aug 28 18:53 1b*
-rw-rw-r-- 1 abheet abheet   439 Aug 28 18:53 1b.c
-rw-rw-r-- 2 abheet abheet     0 Aug  8 23:56 1b.txt
-rw-rw-r-- 2 abheet abheet     0 Aug  8 23:56 hardlink_1b
*/

