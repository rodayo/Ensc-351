/* ***        	                memoryA.c    			    ***	*/
/* This program demonstrates how to create and access a shared memory	*/
/* object. It first requests to open a shared memory object by the name	*/
/* "mem_test_2" for read write operations and for access by everybody.	*/
/* It next sizes the memory block to 4K byts and maps the address of 	*/
/* this	block of physical memroy to its logical memory address space 	*/
/* (where).								*/
/* It then copies a string into this memory block and goes into a 	*/
/* polling loop waiting for the contents of this memory block to be 	*/
/* changed by someone (another process or thread).			*/
/* To demonstrate, first compile and run this program. Nothing should 	*/
/* seem	happening. Open another terminal and type <pidin> to check 	*/
/* system status. You should see this process running. 			*/
/* Next run memoryB which modifies the contents of this shared memory	*/
/* block. You should see memoryA exit the polling loop and quits when 	*/
/* it detects the change in memory contents.				*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define OFLAGS O_CREAT|O_RDWR        
#define SIZE 4096
#define PROT PROT_READ|PROT_WRITE
#define MFLAGS MAP_SHARED

int main(void) 
{
	int mem;
 	char *where;
	printf("Unlinking 'mem_test' object...\n");
	int unlink = shm_unlink("mem_test");
	printf("Result: %d \n\n", unlink);
	// creates an area instead of looking for one, third argument specifys permission.
	// in this case all can access.
	
	printf("Opening 'mem_test' object...\n");
	mem = shm_open("mem_test", OFLAGS, MFLAGS);
	
	// if not -1 then successful
	if(mem != -1)
	{
		// set size before using. Can't resize a shared memory.
		if(ftruncate(mem,SIZE)!=-1)
		{
			// maping area into own address space, 3rd - Protection, 4th - arguments
			where= mmap(0, SIZE, PROT, MFLAGS, mem, 0);
			if(where!=(void*)-1)
			{
				// copy a test string to shared memory
				strcpy(where,"original string");
				printf("A:memory set to '%s'\n",where);
				fflush(stdout);
				// wait until first letter of string is overwritten by another process
				while(*where == 'o')
					sleep(1);
					
				printf("A:memory changed to '%s'\n",where);
				fflush(stdout);
				// unmap shared memory
				munmap(where,SIZE);
			}
			else
			{
				printf("Error .. \n");
			}
			
		}
		else
		{
			printf("Error \n");
		}
		
		//close file descriptor and unlink
		close(mem);
		shm_unlink("mem_test");
	}
	else
	{
		printf("Failed to open 'mem_test' with result %d \n", mem);
	}
	return 0;
}
