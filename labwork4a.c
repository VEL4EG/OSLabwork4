#include "labwork4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

void mainLoop(char *shmaddr, const int fd)
{
	int command;
	char timeStr[50], pidStr[25];

	sprintf(pidStr, "PID: %u ", getpid());

	printf("My %s\n", pidStr);

	printf("Waiting for labwork4b\n");
	
	for (int i = 0; i < ITER_COUNT; i++)
	{
		strcpy(shmaddr, pidStr);
		strcat(shmaddr, "Current time: ");
		getCurrentTime(timeStr);	
		strcat(shmaddr, timeStr);
		
		command = WORK;
		write(fd, &command, sizeof(int));
	
		do
		{
			sleep(1);

			read(fd, &command, sizeof(int));

			if (command == DONE)
			break;
			else
			{
				command = WORK;
				write(fd, &command, sizeof(int));
			}
		}
		while (true);
		
		printf("%s\n", shmaddr);
	}
		
	command = END;
	write(fd, &command, sizeof(int));
}

int main()
{
	return mainFunc(mainLoop);
}
