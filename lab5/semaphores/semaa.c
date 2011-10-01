/* *** 			        semaa.c     			            ***	*/
/* This program demonstrates how to use a semaphore to control the access 	*/
/* of a shared memory block by 2 processes (semaa and semab).          	  	*/
/* Note that the semaphore is implemented with just another shared memory 	*/
/* object, as opposed to using a single shared memory object for both access 	*/
/* control and data.								*/
/* Note also that you have to specify the complete directory path to name    	*/
/* both your shared memory object as well as your semaphore. Use the pwd     	*/
/* command to determine your current working directory. 	         	*/
/* You should inspect the output from these 2 processes carefully so you are 	*/
/* satisfied with what they are supposed to have done.			     	*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
//#include <process.h>
#include <semaphore.h>
#include <sys/mman.h> 

#define OFLAGS	O_RDWR|O_CREAT
#define SIZE		4096
#define PROT		PROT_READ|PROT_WRITE
#define MFLAGS	MAP_SHARED

int main( )
{
	sem_t* sem;
	pid_t pid;
	int shmem, i, s_return;
	char* memLocation;
	
	//Unlink the shared memory object, in case it may have remained linked from previous executions
	printf("Unlinked shared memory object 'shared_memory'...\n");
	int unlink = shm_unlink("shared_memory");
	printf("Result of unlinking: %d \n", unlink);
	
	//Create shared memory object
	shmem = shm_open("shared_memory", OFLAGS, MFLAGS);
	if (shmem == -1)
	{
		printf("shared_memory failed to open...\n");
		fflush(stdout);
		return 0;
	}
	else
	{
		//If successful set the size
		if (ftruncate (shmem, SIZE) == -1)
		{
			printf("Failed to set size for -- shmem -- \n");
			fflush(stdout);
		}
		else
		{
			//If successful map it and get the address
			memLocation = mmap(0,  SIZE, PROT, MFLAGS, shmem, 0);
			if (memLocation == MAP_FAILED)
			{
				printf("Failed to map to shared memory...\n");
				fflush(stdout);
				return 0;
			}
		}	
	}	
	
	//Open a semaphore similar to the way we open a shared memory object and reset its count value
	sem = sem_open("shared_sem", O_CREAT, S_IRWXG|S_IRWXO|S_IRWXU, 0);
	if (sem == (sem_t *)(-1)) 
	{
	   printf ("User: Semaphore failed to open....\n");
	   fflush(stdout);
		 return 0;
	}
	strcpy(memLocation, "START");

	pid = fork();

	//Check process ID
	if(pid > 0)
	{
		printf("I am the original process\n");	
		fflush(stdout);
	}
	else if(pid == 0)
	{
		printf("I am the new process\n");
		fflush(stdout);
		execl("/home/david/git/Ensc-351/lab5/semaphores/semB", (char*) 0);
	}
	else
	{
		printf("Failed to fork a new process... \n");
		return 0;
	}



	if(pid != -1)
	{
		for(i = 0; i < 20; ++i)
		{
			sem_wait(sem);
			printf("A:%s\n", memLocation);
			fflush(stdout);
			sprintf(memLocation, "SHARED %d", i);
			sem_post(sem);
		}
	}
	else
	{
		printf("PID = -1\n");
	}
	
	sem_close(sem);
	s_return = sem_unlink("shared_sem");
	if (s_return == -1)
	{
		printf("a: %s\n", strerror(errno));
		fflush(stdout);
	}
	
	munmap(memLocation, SIZE);
	shm_unlink("shared_memory");
	
	return 0;
}	

