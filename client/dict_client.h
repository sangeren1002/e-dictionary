#ifndef __DIC_CLIENT_H__
#define __DIC_CLIENT_H__

#define BACKLOG 10

typedef struct 
{
	char type;
	char name[128];
	char data[1024];
	char word[512];
	char history[2048];
}MSG;

MSG msg;

char historyBuf[1024];

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/select.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sqlite3.h>

#define PORT 58888

void myTCPCliPreconnect(int *sockfd,struct sockaddr_in *servaddr,struct sockaddr_in *cliaddr,const char *ip_addr,socklen_t *len);

void myTCPCliConnect(int *sockfd,struct sockaddr_in *servaddr,socklen_t len);

void do_clientregister(int sockfd,MSG *msg);

int  do_clientlogin(int sockfd, MSG *msg);

void do_clientquery(int sockfd, MSG *msg);

void do_clienthistory(int sockfd, MSG *msg);

#endif
