/* Name: Abheet Sethi
 * Registration No.: MT2024004
 *
 * Problem Statement: Write a program to display the environment variable of the user
*/

#include<stdio.h>

extern char **environ;

int main()
{
	char **env = environ;

	while(*env)
	{
		printf("%s\n", *env);
		env++;
	}

	return 0;
}
