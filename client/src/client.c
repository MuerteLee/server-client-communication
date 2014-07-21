#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "memory.h"
#include "malloc.h"
#include "tcp.h"
#include "client.h"

int main(int argc, char* argv[])
{
//	memory(argc, argv);	
	malloc_main(argc, argv);
	tcp_main("Hello, It's me!");
	return 0;
}
