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

	printf("Waiting for labwork4a\n");

	bool key = true;
	while (key)
	{
		read(fd, &command, sizeof(int));

		switch (command)
		{
			case DONE:
			{
				command = DONE;
				write(fd, &command, sizeof(int));

				sleep(1);

				break;
			}
			
			case WORK:
			{
				printf("%s\n", shmaddr);
				
				strcpy(shmaddr, pidStr);
				strcat(shmaddr, "Current time: ");
				getCurrentTime(timeStr);	
				strcat(shmaddr, timeStr);
				
				command = DONE;
				write(fd, &command, sizeof(int));

				sleep(1);

				break;
			}

			case END:
			key = false;
			break;
		}
	}
}

int main()
{
	return mainFunc(mainLoop);
}
