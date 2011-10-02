/* ***                   semab.c     				    	    ***	*/
/* This program, in conjunction with semaa.c, demonstrate how to use a		*/
/* semaphore to control the access of a shared memory block.           		*/
/* Note that the semaphore is implemented with just another shared memory    	*/
/* object, as opposed to using a single shared memory object for both access 	*/
/* control and data.								*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/mman.h>

#define OFLAGS	O_RDWR
#define SIZE		4096
#define PROT		PROT_READ|PROT_WRITE
#define MFLAGS	MAP_SHARED

int main(void)
{
	sem_t* sem;
	int shmem, i;
	char* memLocation;

	printf("semB opened! \n");
	fflush(stdout);
	shmem = shm_open("shared_memory", OFLAGS, 0777);
	
	if (shmem == -1)
	{
		printf("shared_memory failed to open...\n");
		fflush(stdout);
		return 0;
	}
	else
	{
		memLocation = mmap(0,  SIZE, PROT, MFLAGS, shmem, 0);
		if (memLocation == MAP_FAILED)
		{
			printf ("Failed to map to shared memory...\n");
			fflush(stdout);
			return 0;
		}
	}	

	sem = sem_open("shared_sem", O_CREAT, S_IRWXG|S_IRWXO|S_IRWXU, 0);
	if (sem == (sem_t *)(-1)) 
	{
	   printf ("User: Semaphore failed to open....\n");
	   fflush(stdout);
		 return 0;
	}

	sem_post(sem);
	for(i = 0; i < 20; ++i)
	{
		sem_wait(sem);
		printf("B:%s\n", memLocation);
		fflush(stdout);
		sprintf(memLocation, "shared %d",i);
		sem_post(sem);
	}
	sem_close(sem);

	munmap(memLocation, SIZE);
	
	return 1;
}
	
