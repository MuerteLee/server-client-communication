#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "server.h"


int main(int argc, char* argv[])
{
	int i, maxi, maxfd, sockfd;
	int nready;
	size_t n;
	fd_set rset, allset;
	int listenfd, connectfd;
	struct sockaddr_in server;

	CLIENT client[FD_SETSIZE];
	char recvbuf[MAXDATASIZE];
	typedef struct SV_INFO{
		char ip1[64];
		int add;
	}ssinfo;
	
//	ssinfo recvbuf[MAXDATASIZE];
	int sin_size;

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Creating socket failed.\n");
		return 0;
	}
#if 1
	int opt = SO_REUSEADDR;
	if((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) == -1)
	{
		printf("setsockopt failed.\n");
		close(listenfd);
		return 0;
	}
	
#endif
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, sizeof(&server.sin_zero));
	//memset(&server.sin_zero, 0, sizeof(&server.sin_zero));

	if(bind(listenfd, (struct sockaddr*)(&server), sizeof(struct sockaddr)) == -1)
	{
		printf("bind failed.\n");
		close(listenfd);
		return 0;
	}

	//call listen
	if(listen(listenfd, BACKLOG) == -1)
	{
		printf("listen error. \n");
		close(listenfd);
		return 0;
	}

	//initialize select
	sin_size = sizeof(struct sockaddr_in);
	maxfd = listenfd;
	maxi = -1;
	for(i = 0; i < FD_SETSIZE; i++)
	{
		client[i].fd = -1;
	}

	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	while(1)
	{
		struct sockaddr_in addr;
		rset = allset;
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
	//	printf("select saw rset actions and the readfset num is %d. \n", nready);

		if(FD_ISSET(listenfd, &rset))
		{
			printf("accept a connection. \n");
			if((connectfd = accept(listenfd, (struct sockaddr*)(&addr), (socklen_t*)&sin_size)) == -1)
			{	
				printf("accept( ) error. \n");
				continue;
			}
			
			if(write(connectfd, "The server has recived message", strlen("The server has recived message")) == -1)
				printf("Write error!\n");

			for(i = 0; i < FD_SETSIZE; i++)
			{
				if(client[i].fd < 0)
				{
					client[i].fd = connectfd;
					client[i].name = (char*)malloc(MAXDATASIZE);
					client[i].addr = addr;
					client[i].data = (char*)malloc(MAXDATASIZE);

					client[i].name[0] = '\0';
					client[i].data[0] = '\0';
					
					printf("You got a connection from %s.\n", inet_ntoa(client[i].addr.sin_addr));
					break;
				}
			}

			printf("add new connect fd.\n");

			if( i == FD_SETSIZE)
				printf("too many clients\n");

			FD_SET(connectfd, &allset);
			if(connectfd > maxfd)
				maxfd = connectfd;

			if(i > maxi)
				maxi = i;
			
			if(--nready <= 0)
				continue;
		}


		for(i = 0; i <= maxi; i++)
		{
			if((sockfd = client[i].fd) < 0)
				continue;
	 
			if(FD_ISSET(sockfd, &rset))
			{
				printf("recv occured for connect fd[%d]. \n", i);
				if((n = recv(sockfd, recvbuf, MAXDATASIZE, 0)) == 0)
				{
					close(sockfd);
//					printf("Client(%s) closed connection. User's data:%s\n", client[i].name, client[i].data);
					FD_CLR(sockfd, &allset);
					client[i].fd = -1;
					free(client[i].name);
					free(client[i].data);
				}else
					process(&client[i], recvbuf, n);

				if(--nready <= 0)
					break;
			}
		}
	}
	close(listenfd);
}

void process(CLIENT* client, char* recvbuf, int len)
{
	char sendbuf[MAXDATASIZE];

#if 1
	recvbuf[len - 1] = '\0';
	if(strlen(client->name) == 0)
	{
		memcpy(client->name, recvbuf, len);
		printf("Client's name is %s.\n", client->name);
		return; 
	}
#endif
	printf("Received client(%s) Recvbuf message: %s\n", client->name, recvbuf);

	savedata(recvbuf, len, client->data);
	int i1;
	for(i1 = 0; i1 < len - 1; i1++)
	{
		sendbuf[i1] = recvbuf[len-i1-2];
	}
	sendbuf[len-1] = '\0';
	send(client->fd, sendbuf, strlen(sendbuf), -1);

}

void savedata(char* recvbuf, int len, char* data)
{
	int start = strlen(data);
	int i;
	for(i = 0; i < len; i++)
	{
		data[start + i] = recvbuf[i];
	}
}
