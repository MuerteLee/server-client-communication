#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "getip.h"
#include "client.h"

typedef struct __INFO{
	char ip1[64];
	int  saddr;
}info;

int my_write(int fd, void *buffer, int length)
{
	int bytes_left;
	int written_bytes;
	char *ptr;

	ptr = buffer;
	bytes_left = length;
	while(bytes_left > 0)
	{
		written_bytes=write(fd, ptr, bytes_left);
		if(written_bytes <= 0)
		{
			if(errno == EINTR)
				written_bytes = 0;
			else
				return -1;
		}
		bytes_left -= written_bytes;
		ptr += written_bytes;
	}
	return 0;
}

int tcp_main (char *buf)
{
	int sockfd;
	struct sockaddr_in dest_addr; //destnation ip info
#if 0
	if(argc != 3)
	{
		printf("useage:socket_client ipaddress port\n eg:socket_client \\par.168.1.158 5555");
		return -1;
	}


	int destport = atoi(argv[2]);

#endif

	if(-1 == (sockfd = socket(AF_INET,SOCK_STREAM,0)) )
	{
		perror("error in create socket\n");
		exit(0);
	}
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(ServerPort);
	dest_addr.sin_addr.s_addr = inet_addr(ServerIp);
	memset(&dest_addr.sin_zero,0,8);
	//connect
	if(-1 == connect(sockfd,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr)))
	{
		perror("connect error\n");
		exit(0);
	}

	send(sockfd,ip_search(), strlen(ip_search()),0);

//	my_write(sockfd, buf, strlen(buf));

#if 0
	int count;
	info inf[10];
	size_t send_count;
	for(count = 0; count < 10; count++)
	{	
		strcpy(inf[count].ip1, "1234");
		inf[count].saddr = malloc(1);
	}

	for(count = 0; count < 10; count++)
		printf("inf[count].ip1=%s, inf[count].saddr=%x\n", inf[count].ip1, inf[count].saddr);
	send_count = send(sockfd, inf,strlen(inf),0);
	printf("send_count = %d\n", send_count);
	sleep(1);
#endif

#if 1
	int count;
	size_t send_count;
	for(count = 0; count < 10; count++)
	{	
		send_count = send(sockfd, buf,strlen(buf)+1,0);
		printf("send_count = %d\n", send_count);
//		sleep(1);
	}
#endif
#if 0
	char buf2[512];
	memset(&buf2, 0, sizeof(buf2));
	char *buf1 = (char*)malloc(sizeof(char*) * 100);
	sprintf(buf2, "%d", buf1);
	int count;
	for(count = 0; count < 10; count++)	
	{
		send(sockfd, "0X1234\n" ,strlen("0X1234\n"),0);
	}
#endif
	getchar();
#if 0
	int flag = 1;
	char buf1[512];
	do{
		memset(&buf1, 0, sizeof(buf1));
		if(read(STDIN_FILENO, buf1, 512) > 0)
			send(sockfd, buf1, sizeof(buf1),0);
		printf("send!\n");

		if(strncmp(buf1, "over", 4) == 0)
			flag = 0;
	}while(flag);
#endif
	close(sockfd);
	return 0;
}


