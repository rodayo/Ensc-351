#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
	printf("Unlinking 'mem_test' object...\n");
	int unlink = shm_unlink("mem_test");
	printf("Result: %d \n\n", unlink);
}
