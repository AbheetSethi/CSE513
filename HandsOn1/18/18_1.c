/* Name: 18_1.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to perform Record locking
  		      a. Implement write lock
  		      b. Implement read lock
 
  		      Create three records in a file. Whenever you access a particular record, first lock it then 
  		      modify/access to avoid race condition

 * Date: 22 Aug 2024
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

/*

I used three terminals windows simultaneously to get the following output:

Terminal 1:						Terminal 2:								Terminal 3:

Command: ./18_1

Command: ./18_2						Command: ./18_2								Command: ./18_2

Output:		        				Output:									Output:
Select train number:					Select train number:							Select train number:
Train number 1   					Train number 1								Train number 1
Train number 2						Train number 2								Train number 2
Train number 3  					Train number 3   							Train number 3
Enter the number: 2					Enter the number: 2							Enter the number: 1	
Before entering critical section			Failed to lock the file: Resource temporarily unavailable		Before entering critical section
Current ticket count: 0														Current ticket count: 0
To book ticket, press enter													To book ticket, press enter

****** After pressing enter in terminal 1 ******

							Command: ./18_2															
Output:							Output:
Ticket booked with number 1				Select train number:
							Train number 1
							Train number 2
							Train number 3
							Enter the number: 2
							Before entering critical section
							Current ticket count: 1
							To book ticket, press enter

							****** After pressing enter in terminal 2 ******

							Output:
							Ticket booked with number 2

																****** After pressing enter in terminal 3 ******

																Output:
																Ticket booked with number 1

*/

