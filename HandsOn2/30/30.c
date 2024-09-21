/* Name: 30.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to remove the message queue.
 * Date: 19 Sept 2024
*/

#include <sys/types.h> // Import for `ftok`, `shmget`, `shmat`
#include <sys/ipc.h>   // Import for `ftok`, `shmget`
#include <sys/shm.h>   // Import for `shmget`, `shmat`
#include <unistd.h>    // Import for `_exit`
#include <stdio.h>     

void main()
{
    key_t shmKey;         // Key used to create / access Shared Memory
    int shmIdentifier;    // Identifier for the Shared Memory
    ssize_t shmSize = 20; // Size of the Shared Memory
    char *shmPointer, *rdOnlyShmPointer;

    shmKey = ftok(".", 1);

    if (shmKey == -1)
    {
        perror("Error while computing key!");
        _exit(0);
    }

    shmIdentifier = shmget(shmKey, shmSize, IPC_CREAT | 0700); // Create the shared memory if it doesn't exist; if it exists use the existing one

    if (shmIdentifier == -1)
    {
        perror("Error while getting Shared Memory!");
        _exit(0);
    }
    // ======================== 1 ========================
    shmPointer = shmat(shmIdentifier, (void *)0, 0);

    if (*shmPointer == -1)
    {
        perror("Error while attaching address space!");
        _exit(0);
    }

    printf("Press enter to write to the shared memory!\n");
    getchar();

    sprintf(shmPointer, "MT2024004"); // Writing to the shared memory

    printf("Press enter to read from the shared memory!\n");
    getchar();

    printf("Data from shared memory: %s\n", shmPointer); // Reading from the shared memory

    // ===================================================

    // ======================== 2 ========================

    rdOnlyShmPointer = shmat(shmIdentifier, (void *)0, SHM_RDONLY);
    if (*rdOnlyShmPointer == -1)
    {
        perror("Error while attaching address space!");
        _exit(0);
    }
    
    // printf("Press enter to write to the shared memory!\n");
    // getchar();

    // The below line will cause a segmentation fault (uncomment to get the required output)
    // sprintf(rdOnlyShmPointer, "MT2024004"); // Writing to the shared memory

    printf("Press enter to read from the shared memory!\n");
    getchar();

    printf("Data from shared memory: %s\n", rdOnlyShmPointer); // Reading from the shared memory

    // ===================================================

    // ======================== 3 ========================
    printf("Detaching pointer to shared memory!\n");
    shmdt(shmPointer);       // Dettach pointer to Shared Memory
    shmdt(rdOnlyShmPointer); // Dettach pointer to Shared Memory
    // ===================================================

    // ======================== 4 ========================
    printf("Press enter to delete the shared memory!\n");
    getchar();

    // Delete Shared Memory
    shmctl(shmIdentifier, IPC_RMID, NULL);
    // ===================================================
}

/*

Command: ./30

Output:
Press enter to write to the shared memory!

Press enter to read from the shared memory!

Data from shared memory: MT2024004
Press enter to read from the shared memory!

Data from shared memory: MT2024004
Detaching pointer to shared memory!
Press enter to delete the shared memory!

*/