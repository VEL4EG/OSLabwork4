#define MEM_SIZE 100
#define ITER_COUNT 5

enum Commands
{
	WORK,
	DONE,
	END
};

// on succes returns descriptor, -1 on failure
int prepareFifo();
// on succes returns shmid and shmaddr will be set to the address
// of the the attached shared memory segment; on error -1 is returned
int prepareSharedMemory(void **shmaddr);
void getCurrentTime(char *str);
// prepares FIFO and share memory, calls mainLoop and frees resources
int mainFunc(void (*mainLoop)(char *shmaddr, const int fd));
