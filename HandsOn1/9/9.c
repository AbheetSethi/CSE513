/* Name: Abheet Sethi
 * Registration Number: MT2024004
 
 * Problem Statement: Write a program to print the following information about a given file.
 
                      a. inode
                      b. number of hard links
                      c. uid
                      d. gid
                      e. size
                      f. block size
                      g. number of blocks
                      h. time of last access
                      i. time of last modification
                      j. time of last change
 */

#include<stdio.h>
#include<sys/stat.h>
#include<time.h>

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s <filename> \n", argv[0]);
		return 1;
	}

	const char* filename = argv[1];
	struct stat fileStat;

	if (stat(filename, &fileStat) == -1)
	{
		perror("Error retrieving file information");
		return 1;
	}

	printf("File: %s\n", filename);
	printf("Inode: %lu\n", fileStat.st_ino);
	printf("Number of hard links: %lu\n", fileStat.st_nlink);
	printf("UID: %u\n", fileStat.st_uid);
	printf("GID: %u\n",fileStat.st_gid);
	printf("Size: %ld bytes\n", fileStat.st_size);
	printf("Block size: %ld bytes\n", fileStat.st_blksize);
	printf("Number of blocks: %ld\n", fileStat.st_blocks);
	printf("Time of last access: %s", ctime(&fileStat.st_atime));
	printf("Time of last modification: %s", ctime(&fileStat.st_mtime));
	printf("Time of last change: %s", ctime(&fileStat.st_ctime));

	return 0;
}
