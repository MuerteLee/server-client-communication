#ifndef __SERVER_H
#define __SERVER_H

#define PORT 5556
#define BACKLOG 5
#define MAXDATASIZE 1024

typedef struct _CLIENT{
	int fd;
	char* name;
	struct sockaddr_in addr;
	char *data;
}CLIENT;

void process_cli(CLIENT *client, char *recvbuf, int len);
void savedata(char* recvbuf, int len, char* data);

#endif
