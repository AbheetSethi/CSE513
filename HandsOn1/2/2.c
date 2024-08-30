/* Name: 2.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a Simple Program to execute in an infinite loop at the background. 
 		      Go to /proc directory and identify all the process related information in 
		      the corresponding proc directory.

 * Date: 8 Aug 2024
*/


#include<stdio.h>

int main()
{
	for(;;);

	return 0;
}

/*

Command: ./2 &

Output:

[1] 5984

Command: cd /proc
         cd 5984

now we can use various commands here to get the process related information:

`cwd`: A symbolic link to the current working directory of the process.
`status`: Contains human-readable information about the process status
`maps`: Contains memory maps of the process
`fd/`: A directory containing symbolic links to the file descriptors opened by
the process
`exe`: A symbolic link to the executable of the process
`environ`: Contains the environment variables of the process

*/
