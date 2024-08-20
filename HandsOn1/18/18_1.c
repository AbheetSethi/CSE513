/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to perform Record locking
  		      a. Implement write lock
  		      b. Implement read lock
 
  		      Create three records in a file. Whenever you access a particular record, first lock it then 
  		      modify/access to avoid race condition
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

struct db
{
	int train_num;
	int ticket_count;
};

int main()
{
	int i, fd;
	struct db db[3];

	for(i=0; i<3; i++)
	{
		db[i].train_num = i + 1;
		db[i].ticket_count = 0;
	}

	fd = open("18.txt", O_RDWR | O_CREAT, 0666);
	if(fd < 0)
	{
		perror("Failed to open file");
		return 1;
	}

	write(fd, db, sizeof(db));
	close(fd);

	return 0;
}
