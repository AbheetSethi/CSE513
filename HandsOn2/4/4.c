/* Name: 4.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to measure how much time is taken to execute 100 getppid() system call. Use time stamp counter.

 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

// Import for __rdtsc()
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

int main() {
    uint64_t start, end;
    pid_t pid;
    int iter = 0;
    struct timeval tv;

    // Measure start TSC value
    start = __rdtsc();

    // Execute 100 getpid() system calls
    while (iter < 100) {
        pid = getpid();
        iter++;
    }

    // Measure end TSC value
    end = __rdtsc();

    // Calculate the number of CPU cycles taken
    uint64_t cycles_taken = end - start;

    // Convert cycles to nanoseconds assuming a fixed CPU frequency
    // You need to know the CPU frequency for accurate conversion
    // Example: Assume a 2.4 GHz CPU
    double cpu_frequency_ghz = 2.4;
    double cycles_per_second = cpu_frequency_ghz * 1e9;
    double time_ns = (cycles_taken / cycles_per_second) * 1e9;

    // Output results
    printf("PID: %d\n", pid);
    printf("Time taken: %.2f ns\n", time_ns);

    return 0;
}

/*

Command: ./4

Output:
PID: 8912
Time taken: 46241.67 ns

*/