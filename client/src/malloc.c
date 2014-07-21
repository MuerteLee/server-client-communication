#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _MEMINFO_DATA{
	size_t address;
	size_t size;
}malloc_data;

int malloc_main(int argc, char* argv[])
{
	int kb_tal = 1024;
	int i;
	
	malloc_data mallocinfo[1024];

	for(i =0; i < 1024; i++)
	{	
		mallocinfo[i].address = malloc(100) ;
//		printf("address = %d\n", mallocinfo[i].address);
//		tcp_main("\n It's me\n");
	}
	return 0;
}
