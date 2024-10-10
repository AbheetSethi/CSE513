# Hands ON List 1 
## File Management

1. Create the following types of a files using (i) shell command (ii) system call
 a. soft link (symlink system call)
 b. hard link (link system call)
 c. FIFO (mkfifo Library Function or mknod system call)

2. Write a simple program to execute in an infinite loop at the background. Go to /proc directory and identify all the process related information in the corresponding proc directory. 

3. Write a program to create a file and print the file descriptor value. Use creat() system call

4. Write a program to open an existing file with read write mode. Try O_EXCL flag also.
 
5. Write a program to create five new files with infinite loop. Execute the program in the background and check the file descriptor table at /proc/pid/fd.

6. Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
   
7. Write a program to copy file1 into file2 ($cp file1 file2).
   
8. Write a program to open a file in read only mode, read line by line and display each line as it is read. Close the file when end of file is reached.

9. Write a program to print the following information about a given file.
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

10. Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10 bytes (use lseek) and write again 10 bytes. 
 a. check the return value of lseek
 b. open the file with od and check the empty spaces in between the data. 

11. Write a program to open a file, duplicate the file descriptor and append the file with both the descriptors and check whether the file is updated properly or not.
 a. use dup
 b. use dup2
 c. use fcntl

12. Write a program to find out the opening mode of a file. Use fcntl.

13. Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to verify whether the data is available within 10 seconds or not (check in $man 2 select).

14. Write a program to find the type of a file.
 a. Input should be taken from command line.
 b. program should be able to identify any type of a file. 

15. Write a program to display the environmental variable of the user (use environ).

## File Locking
  
16. Write a program to perform mandatory locking.
 a. Implement write lock
 b. Implement read lock

17. Write a program to simulate online ticket reservation. Implement write lock
    Write a program to open a file, store a ticket number and exit.
    Write a separate program, to open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.

18. Write a program to perform Record locking.
 a. Implement write lock
 b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access to avoid race condition.

## Process Management

19. Write a program to find out time taken to execute getpid system call. Use time stamp counter.  

20. Find out the priority of your running program. Modify the priority with nice command. 

21. Write a program, call fork and print the parent and child process id. 

22. Write a program, open a file, call fork, and then write to the file by both the child as well as the parent processes. Check output of the file.  

23. Write a program to create a Zombie state of the running program. 

24. Write a program to create an orphan process. 

25. Write a program to create three child processes. The parent should wait for a particular child (use waitpid system call). 

26. Write a program to execute an executable program. 
a. use some executable program 
b. pass some input to an executable program. (for example execute an executable of $./a.out name) 

27. Write a program to execute ls -Rl by the following system calls 
a. execl 
b. execlp 
c. execle 
d. execv 
e. execvp 

28. Write a program to get maximum and minimum real time priority. 

29. Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO, SCHED_RR). 

30. Write a program to run a script at a specific time using a Daemon process.

# Hands ON List 2

1. Write a separate program (for each time domain) to set a interval timer in 10 sec and 
10 micro second
 a. ITIMER_REAL
 b. ITIMER_VIRTUAL
 c. ITIMER_PROF
 
2. Write a program to print the system resource limits. Use getrlimit system call.

3. Write a program to set (any one) system resource limit. Use setrlimit system call.

4. Write a program to measure how much time is taken to execute 100 getppid ( ) system call. Use time stamp counter.

5. Write a program to print the system limitation of 
 a. maximum length of the arguments to the exec family of functions. 
 b. maximum number of simultaneous process per user id. 
 c. number of clock ticks (jiffy) per second. 
 d. maximum number of open files
 e. size of a page
 f. total number of pages in the physical memory
 g. number of currently available pages in the physical memory.
 
6. Write a simple program to create three threads.

7. Write a simple program to print the created thread ids.

8. Write a separate program using signal system call to catch the following signals.
 a. SIGSEGV
 b. SIGINT
 c. SIGFPE
 d. SIGALRM (use alarm system call)
 e. SIGALRM (use setitimer system call)
 f. SIGVTALRM (use setitimer system call)
 g. SIGPROF (use setitimer system call)
 
9. Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - Use signal system call.

10. Write a separate program using sigaction system call to catch the following signals.
 a. SIGSEGV
 b. SIGINT
 c. SIGFPE
 
11. Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - use sigaction system call.

12. Write a program to create an orphan process. Use kill system call to send SIGKILL signal to the parent process from the child process.

13. Write two programs: first program is waiting to catch SIGSTOP signal, the second program will send the signal (using kill system call). Find out whether the first program is able to catch the signal or not.

14. Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.

15. Write a simple program to send some data from parent to the child process.

16. Write a program to send and receive data from parent to child vice versa. Use two way communication.

17. Write a program to execute ls -l | wc.
 a. use dup
 b. use dup2
 c. use fcntl

18. Write a program to find out total number of directories on the pwd. execute ls -l | grep ^d | wc ? Use only dup2.

19. Create a FIFO file by
 a. mknod command
 b. mkfifo command
 c. use strace command to find out, which command (mknod or mkfifo) is better.
 c. mknod system call
 d. mkfifo library function

20. Write two programs so that both can communicate by FIFO -Use one way communication.

21. Write two programs so that both can communicate by FIFO -Use two way communications.

22. Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.

23. Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).

24. Write a program to create a message queue and print the key and message queue id.

25. Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
 a. access permission
 b. uid, gid
 c. time of last message sent and received 
 d. time of last change in the message queue
 d. size of the queue
 f. number of messages in the queue 
 g. maximum number of bytes allowed 
 h. pid of the msgsnd and msgrcv

26. Write a program to send messages to the message queue. Check $ipcs -q

27. Write a program to receive messages from the message queue.
 a. with 0 as a flag
 b. with IPC_NOWAIT as a flag

28. Write a program to change the exiting message queue permission. (use msqid_ds structure)

29. Write a program to remove the message queue.

30. Write a program to create a shared memory.
 a. write some data to the shared memory
 b. attach with O_RDONLY and check whether you are able to overwrite.
 c. detach the shared memory
 d. remove the shared memory

31. Write a program to create a semaphore and initialize value to the semaphore.
 a. create a binary semaphore
 b. create a counting semaphore

32. Write a program to implement semaphore to protect any critical section.
 a. rewrite the ticket number creation program using semaphore
 b. protect shared memory from concurrent write access
 c. protect multiple pseudo resources ( may be two) using counting semaphore
 d. remove the created semaphore 

33. Write a program to communicate between two machines using socket.

34. Write a program to create a concurrent server.
 a. use fork
 b. use pthread_create
