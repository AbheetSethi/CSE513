/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to perform Record locking
  		      a. Implement write lock
  		      b. Implement read lock
 
  		      Create three records in a file. Whenever you access a particular record, first lock it then
  		      modify/access to avoid race condition.
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
	int fd, input;
	struct db db;

	fd = open("18.txt", O_RDWR);
	if(fd < 0)
	{
		perror("Failed to open file");
		return 1;
	}

	printf("Select train number:\n" );
	printf("Train number 1\n");
	printf("Train number 2\n");
	printf("Train number 3\n");
	printf("Enter the number: ");
	scanf("%d", &input);

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (input -1) * sizeof(db);
	lock.l_len = sizeof(db);
	lock.l_pid = getpid();

	if(fcntl(fd, F_SETLK, &lock) == -1)
	{
		perror("Failed to lock the file");
		close(fd);
		return 1;
	}

	lseek(fd, (input - 1) * sizeof(db), SEEK_SET);
	read(fd, &db, sizeof(db));

	printf("Before entering crititcal section\n");
	printf("Current ticket count: %d\n", db.ticket_count);

	db.ticket_count++;

	lseek(fd, (input - 1) * sizeof(db), SEEK_SET);
	write(fd, &db, sizeof(db));
	printf("To book ticket, press enter\n");
	getchar();
	getchar();

	lock.l_type = F_UNLCK;
	if(fcntl(fd, F_SETLK, &lock) == -1)
	{
		perror("Failed to unlock the file");
	}

	printf("Ticket booked with number %d\n", db.ticket_count);

	close(fd);
	return 0;
}
