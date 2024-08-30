/* Name: 17_1.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to simulate online ticket reservation. Implement write lock.
  		      
  		      Write a program to open a file, store a ticket number and exit.
  		      
  		      Write a separate program, to open the file, implement write lock, read the ticket
  		      number, increment the number and print the new ticket number then close the file.

 * Date: 22 Aug 2024
*/ 

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	int fd;
	struct
	{
		int ticket_no;
	}db;

	db.ticket_no = 10;

	fd = open("db", O_CREAT | O_RDWR, 0744);

	write(fd, &db, sizeof(db));

	close(fd);

	fd = open("db", O_RDONLY);

	read(fd, &db, sizeof(db));

	printf("Ticket no: %d\n", db.ticket_no);

	close(fd);

	return 0;
}

/*

I used three terminals windows simultaneously to get the following output:

Terminal 1:							Terminal 2:						Terminal 3:

Command: ./17_1

Output:
Ticket no: 10

Command: ./17_2							Command: ./17_2						Command: ./17_2

Output:								Output:							Output:
Before entering into critical section				Before entering into critical section			Before entering into criticcal section
Inside the critical section
Current ticket number: 10
Press enter to unlock

****** After pressing enter in Terminal 1 ******

Output:								Output:							Output:
Exited critical section						Inside the critcal section				
								Current ticket number: 11
								Press enter to unlock

								****** After pressing enter in Terminal 2 ******

Output:								Output:							Output:
								Exited critical section					Inside the critical section
															Current ticket number: 12
															Press enter to unlock
*/
