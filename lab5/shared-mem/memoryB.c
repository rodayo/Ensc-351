/* ***                           memoryB.c  			    	    *** */
/* This program works in conjunction with memoryA to demonstrate access 	*/
/* of a shared memory object. It first opens the shared memory object has  	*/
/* been previously created by memoryA and then modifies its contents.		*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define OFLAGS O_CREAT|O_RDWR
#define SIZE 4096
#define PROT PROT_READ|PROT_WRITE
#define MFLAGS MAP_SHARED

int main(void) 
{
	
	int mem;
	//volatile char *where;
	char *where;
	//open the memory area
	printf("Opening 'mem_test' object...\n");
	mem = shm_open("mem_test", O_RDWR, 0777);
	
	//if not -1 then successful
	if(mem != -1)
	{
		//maping area into own address space.
		//3rd - Protection, 4th - arguments
		where = mmap(0, SIZE, PROT, MFLAGS, mem, 0);
		if(where != (void*)-1)
		{
			//print out contents of memory area
			printf("B:memory contains'%s'\n", where);
			//overwrite with new string
			strcpy(where, "new string");
		}
		//close file descriptor and unlink
		close(mem);
		munmap(where, SIZE);
	}
	else
	{
		printf("Failed to open 'mem_test' with result %d \n", mem);
	}
}

