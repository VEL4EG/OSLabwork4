#include "labwork4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/time.h>

int prepareFifo()
{
	int fd;
	
	if ((mkfifo("Fifo", 0777) == -1) && (errno != EEXIST))
	{
		printf("Error: %s\n", strerror(errno));
		return -1;
	}
	
	printf("FIFO has been created\n");
	
	fd = open("Fifo", O_RDWR);

	if (fd == -1)
	{
		printf("Error: %s\n", strerror(errno));
		remove("Fifo");
		return -1;
	}

	printf("FIFO has been opened\n");
	
	return fd;
}

int prepareSharedMemory(void **shmaddr)
{
	int shmid;
	key_t key = ftok("Fifo", 1);

	shmid = shmget(key, MEM_SIZE, IPC_CREAT | 0777);

	if (shmid == -1)
	{
		printf("Error: %s\n", strerror(errno));
		return -1;
	}

	printf("Shared memory has been created\n");
	
	*shmaddr = shmat(shmid, NULL, 0);

	if (shmaddr == (void*)-1)
	{
		printf("Error: %s\n", strerror(errno));
		shmctl(shmid, IPC_RMID, 0);
		return -1;
	}
	
	printf("Shared memory has been attached\n");

	return shmid;
}

void getCurrentTime(char *str)
{
	static struct timeval timer;
	
	gettimeofday(&timer, NULL);
	strftime(str, 25, "%T.", localtime(&timer.tv_sec));
	sprintf(str + strlen(str), "%ld", timer.tv_usec);
}

int mainFunc(void (*mainLoop)(char *shmaddr, const int fd))
{
	int fd, shmid;
	char *shmaddr;

	fd = prepareFifo();

	if (fd == -1)
	return -1;

	shmid = prepareSharedMemory((void**)&shmaddr);

	if (shmid == -1)
	{
		close(fd);
		remove("Fifo");
		return -1;
	}
	
	mainLoop(shmaddr, fd);
	
	close(fd);
	remove("Fifo");
	
	shmdt(shmaddr);
	shmctl(shmid, IPC_RMID, 0);
	
	return 0;	
}
