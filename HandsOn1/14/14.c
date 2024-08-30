/* Name: 14.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to find the type of a file. 
  		      
  		      a. Input should be taken from command line.
  		      b. Program should be able to find any type of file

 * Date: 22 Aug 2024
*/

#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>

void print_file_type(const char* path)
{
	struct stat file_stat;

	if(stat(path, &file_stat) == -1)
	{
		perror("stat");
		exit(EXIT_FAILURE);
	}

	if(S_ISREG(file_stat.st_mode))
	{
		printf("%s is a regular file \n", path);
	}

	else if(S_ISDIR(file_stat.st_mode))
	{
		printf("%s is a directory \n", path);
	}

	else if(S_ISLNK(file_stat.st_mode))
	{
		printf("%s is a symbolic link \n", path);
	}

	else if(S_ISCHR(file_stat.st_mode))
	{
		printf("%s is a character device \n", path);
	}

	else if(S_ISBLK(file_stat.st_mode))
	{
		printf("%s is a block device \n", path);
	}

	else if(S_ISFIFO(file_stat.st_mode))
	{
		printf("%s is a FIFO (named pipe) \n", path);
	}

	else if(S_ISSOCK(file_stat.st_mode))
	{
		printf("%s is a socket \n", path);
	}

	else
	{
		printf("%s is of an unknown file type \n", path);
	}
}

int main(int argc, char *agrv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s 14.txt \n", agrv[0]);
		exit(EXIT_FAILURE);
	}

	print_file_type(agrv[1]);

	return 0;
}

/*

Command: ./14 14.txt

Output:
14.txt is a regular file

Command: ./14 ../

Output:
../ is a directory

*/
