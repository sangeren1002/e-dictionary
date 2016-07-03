#ifndef __DIC_SERVER_H__
#define __DIC_SERVER_H__

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


void myTCPSerAccept(int *sockfd,struct sockaddr_in *servaddr,int *connfd,socklen_t *len);

void myTCPSerPreconnect(int *sockfd,struct sockaddr_in *servaddr,const char *ip_addr,socklen_t *len);

int callbackRegister(void *connfd,int f_num,char **f_value,char **f_name);

void do_serverregister(int connfd,MSG *msg,sqlite3 *infodb);

int callbacklogin(void *Notused,int f_num,char **f_value,char **f_name);

void do_serverlogin(int connfd,MSG *msg,sqlite3 *infodb);

int callbackquery(void *Notused,int f_num,char **f_value,char **f_name);

void do_serverquery(int connfd,MSG *msg,sqlite3 *dictdb,sqlite3 *infodb);

int callbackhistory(void *Notused,int f_num,char **f_value,char **f_name);

void do_serverhistory(int connfd,MSG *msg,sqlite3 *infodb);



#endif
