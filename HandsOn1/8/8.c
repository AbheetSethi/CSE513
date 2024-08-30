/* Name: 8.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Description: Write a program to open a file in read only mode read line by line and display each line
 * 	        as it is read. Close the file when end of file is reached
 * 
 * Date: 8 Aug, 2024
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    FILE* fd = fopen("8.txt", "r");
    if (fd == NULL) {
        perror("Program");
        exit(1);
    }

    int c;
    char buffer[1024];
    int k = 0;

    while ((c = fgetc(fd)) != EOF) {
        buffer[k++] = c;
        if (c == '\n' || k == 1023) {  
            buffer[k] = '\0';  
            write(1, buffer, k);
            k = 0;  
        }
    }

    if (k > 0) { 
        buffer[k] = '\0';
        write(1, buffer, k);
    }

    fclose(fd);
    return 0;
}

/*

Command: ./8

Output:
Name: Abheet Sethi
Registration No.: MT2024004

Content: This is program number 8

*/
