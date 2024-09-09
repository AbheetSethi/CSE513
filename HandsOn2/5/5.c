/* Name: 5.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to print the system limitation of 
                        a. maximum length of the arguments to the exec family of functions. 
                        b. maximum number of simultaneous process per user id. 
                        c. number of clock ticks (jiffy) per second. 
                        d. maximum number of open files
                        e. size of a page
                        f. total number of pages in the physical memory
                        g. number of currently available pages in the physical memory. 

 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Function to get the maximum length of arguments to the exec family of functions
void print_max_arg_length() 
{
    long max_arg_length = sysconf(_SC_ARG_MAX);
    
    if (max_arg_length == -1) 
    {
        perror("sysconf(_SC_ARG_MAX) error");
    } 
    
    else 
    {
        printf("Maximum length of arguments to exec functions: %ld bytes\n", max_arg_length);
    }
}

// Function to get the maximum number of simultaneous processes per user id
void print_max_processes() 
{
    struct rlimit rl;
    
    if (getrlimit(RLIMIT_NPROC, &rl) == -1) 
    {
        perror("getrlimit(RLIMIT_NPROC) error");
    } 
    
    else 
    {
        printf("Maximum number of simultaneous processes per user: Soft limit: %ld, Hard limit: %ld\n", rl.rlim_cur, rl.rlim_max);
    }
}

// Function to get the number of clock ticks (jiffy) per second
void print_ticks_per_second() 
{
    long ticks_per_sec = sysconf(_SC_CLK_TCK);
    
    if (ticks_per_sec == -1) 
    {
        perror("sysconf(_SC_CLK_TCK) error");
    } 
    
    else 
    {
        printf("Number of clock ticks per second: %ld\n", ticks_per_sec);
    }
}

// Function to get the maximum number of open files
void print_max_open_files() 
{
    struct rlimit rl;
    
    if (getrlimit(RLIMIT_NOFILE, &rl) == -1) 
    {
        perror("getrlimit(RLIMIT_NOFILE) error");
    } 
    
    else 
    {
        printf("Maximum number of open files: Soft limit: %ld, Hard limit: %ld\n", rl.rlim_cur, rl.rlim_max);
    }
}

// Function to get the size of a page
void print_page_size() 
{
    long page_size = sysconf(_SC_PAGESIZE);
    
    if (page_size == -1) 
    {
        perror("sysconf(_SC_PAGESIZE) error");
    } 
    
    else 
    {
        printf("Size of a page: %ld bytes\n", page_size);
    }
}

// Function to get the total number of pages in physical memory
void print_total_pages() 
{
    struct sysinfo info;
    
    if (sysinfo(&info) == -1) 
    {
        perror("sysinfo() error");
    } 
    
    else 
    {
        printf("Total number of pages in physical memory: %lu\n", info.totalram / sysconf(_SC_PAGESIZE));
    }
}

// Function to get the number of currently available pages in physical memory
void print_available_pages() 
{
    struct sysinfo info;
    
    if (sysinfo(&info) == -1) 
    {
        perror("sysinfo() error");
    } 
    
    else 
    {
        printf("Number of currently available pages in physical memory: %lu\n", info.freeram / sysconf(_SC_PAGESIZE));
    }
}

int main() {
    print_max_arg_length();
    print_max_processes();
    print_ticks_per_second();
    print_max_open_files();
    print_page_size();
    print_total_pages();
    print_available_pages();

    return 0;
}

/*

Command: ./5

Output:

Maximum length of arguments to exec functions: 2097152 bytes
Maximum number of simultaneous processes per user: Soft limit: 62724, Hard limit: 62724
Number of clock ticks per second: 100
Maximum number of open files: Soft limit: 1024, Hard limit: 1048576
Size of a page: 4096 bytes
Total number of pages in physical memory: 4033083
Number of currently available pages in physical memory: 2310316

*/