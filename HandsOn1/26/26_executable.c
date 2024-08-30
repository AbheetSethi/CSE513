#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Kindly enter the input \n");
        return 1;
    }

    printf("Your roll number is: %s!\n", argv[1]);
    return 0;
}
