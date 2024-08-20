/* Name: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to simulate online ticket reservation. Implement write lock.
 
  		      Write a program to open a file, store a ticket number and exit.
 
  		      Write a separate program, to open the file, implement write lock, read the ticket
  		      number, increment the number and print the new ticket number then close the file.
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	struct flock lock;
	int fd;
	struct
	{
		int ticket_no;
	} db;

	fd = open("db", O_RDWR);

	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;

	printf("Before entering into crititcal section\n");

	fcntl(fd, F_SETLKW, &lock);
	printf("Inside the critical section\n");

	read(fd, &db, sizeof(db));
	printf("Current ticket number: %d\n", db.ticket_no);

	db.ticket_no++;

	lseek(fd, 0, SEEK_SET);

	write(fd, &db, sizeof(db));

	printf("Press enter to unlock\n");
	getchar();

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	printf("Exited critical section\n");

	close(fd);

	return 0;
}
