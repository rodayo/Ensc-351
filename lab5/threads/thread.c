/* ***                   	thread.c 			    ***  	*/
/* This program demonstrates how to create a thread from a process.    		*/
/* The process (or the main thread) first creates a thread (my_thread) which	*/
/* is put into a ready queue upon creation. The main thread then prints a	*/
/* message and sleeps (gives up the CPU) for 20 seconds.			*/
/* The new thread, in the meantime, goes into a loop for 5 iterations before	*/
/* it dies.									*/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// Child Thread code 
void *my_thread (void *not_used)
{
	int	x;
	
	for (x = 0; x < 5; x++)	
	{
		printf ("I am a thread at %d\n", x);
		fflush(stdout);
		sleep (1);
	}
	return (NULL);
}	


int main(void) 
{
	pthread_attr_t attr;
	pthread_t thread;
	
	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
      
	pthread_create(&thread, &attr, my_thread, NULL);
	printf("Thread running, I am sleeping\n");
	fflush(stdout);
	sleep (20); // 15 s more after printing stops
	return (EXIT_SUCCESS);
}





