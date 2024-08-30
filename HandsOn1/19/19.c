/* Name: 19.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to find out time taken to execute getpid system call. Use time stamp counter.

 * Date: 22 Aug 2024
*/

#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main() {
    struct timespec start, end;
    long long elapsed_time_ns;

    clock_gettime(CLOCK_MONOTONIC, &start);

    int pid = getpid();

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_time_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    printf("PID: %d\n", pid);
    printf("Time taken to execute getpid: %lld ns\n", elapsed_time_ns);

    return 0;
}

/*

Command: ./19

Output:
PID: 5500
Time taken to execute getpid: 8394 ns

*/
