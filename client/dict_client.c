#include "dict_client.h"
#include "color.h"




void myTCPCliPreconnect(int *sockfd,struct sockaddr_in *servaddr,struct sockaddr_in *cliaddr,const char *ip_addr,socklen_t *len)
{
    if(NULL == sockfd || NULL == servaddr || NULL == ip_addr || NULL == len)
    {
        printf("%ssyntax error:mind the pointer argument%s\n",GREEN,NONE);
        exit(EXIT_FAILURE);
    }

    int retbind;
    int optval = 1;
    int retsetopt;
    *sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == *sockfd)
    {
        perror("fail to socket");
        exit(EXIT_FAILURE);
    }
    printf("%ssocket open successfully%s\n",GREEN,NONE);

    (*servaddr).sin_family = AF_INET;
    (*servaddr).sin_port = htons(PORT);
    (*servaddr).sin_addr.s_addr = inet_addr(ip_addr);

    retsetopt = setsockopt(*sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
    if(-1 == retsetopt)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    (*cliaddr).sin_family = AF_INET;
    (*cliaddr).sin_port = htons(PORT+1);
    (*cliaddr).sin_addr.s_addr = inet_addr("0.0.0.0");

    retbind = bind(*sockfd,(struct sockaddr *)cliaddr,*len);
    if(-1 == retbind)
    {
        perror("fail to bind");
        exit(EXIT_FAILURE);
    }
    printf("%sbind successfully%s\n",GREEN,NONE);

}

void myTCPCliConnect(int *sockfd,struct sockaddr_in *servaddr,socklen_t len)
{
    int retconn;
    retconn = connect(*sockfd,(struct sockaddr *)servaddr,len);
    if(-1 == retconn)
    {
        perror("fail to connect");
        exit(EXIT_FAILURE);
    }
    printf("%sconnect successfully%s\n",GREEN,NONE);
}




void do_clientregister(int sockfd, MSG *msg)
{
	ssize_t retsend;
	ssize_t retrecv;
	size_t msglen = sizeof(MSG);
	msg->type = 'R';
	strcpy(msg->history,"Start");
	char username[128];
	char password[128];
	char buf[128];


	printf("please input the username:");
	fgets(username,128,stdin);
	username[strlen(username)-1] = '\0';

	strcpy(msg->name,username);

	printf("Please input the password:");
	fgets(password,128,stdin);
	password[strlen(password)-1] = '\0';

	strcpy(msg->data,password);

	retsend = send(sockfd,msg,msglen,0);
	if(-1 == retsend)
	{
		perror("fail to send.\n");
		exit(EXIT_FAILURE);
	}

	retrecv = recv(sockfd,buf,sizeof(buf),0);
	if(-1 == retrecv)
	{
		perror("fail to recv.\n");
		exit(EXIT_FAILURE);
	}
	printf("%s%s%s\n",GREEN,buf,NONE);
}



int  do_clientlogin(int sockfd, MSG *msg)
{
	ssize_t retsend;
	ssize_t retrecv;
	size_t msglen = sizeof(MSG);
	msg->type = 'L';
	char username[128];
	char password[128];
	char buf[128];


	printf("please input the username:");
	fgets(username,128,stdin);
	username[strlen(username)-1] = '\0';

	strcpy(msg->name,username);

	printf("Please input the password:");
	fgets(password,128,stdin);
	password[strlen(password)-1] = '\0';

	strcpy(msg->data,password);

	retsend = send(sockfd,msg,msglen,0);
	if(-1 == retsend)
	{
		perror("fail to send.\n");
		exit(EXIT_FAILURE);
	}

	
	retrecv = recv(sockfd,buf,sizeof(buf),0);
	if(-1 == retrecv)
	{
		perror("fail to recv.\n");
		exit(EXIT_FAILURE);
	}
	printf("%s%s%s\n",GREEN,buf,NONE);

	if(strcmp(buf,"Login successfully.") == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}



void do_clientquery(int sockfd, MSG *msg)
{
	ssize_t retsend;
	ssize_t retrecv;
	size_t msglen = sizeof(MSG);
	msg->type = 'Q';
	char wordBuf[128];
	char buf[512];


	printf("Please input the word:");
	fgets(wordBuf,128,stdin);
	wordBuf[strlen(wordBuf)-1] = '\0';

	strcpy(msg->word,wordBuf);

	retsend = send(sockfd,msg,msglen,0);
	if(-1 == retsend)
	{
		perror("fail to send.\n");
		exit(EXIT_FAILURE);
	}

	retrecv = recv(sockfd,msg,sizeof(MSG),0);
	if(-1 == retrecv)
	{
		perror("fail to recv.\n");
		exit(EXIT_FAILURE);
	}
	printf("%s%s%s\n",GREEN,msg->word,NONE);
}



void do_clienthistory(int sockfd, MSG *msg)
{
	ssize_t retsend;
	ssize_t retrecv;
	size_t msglen = sizeof(MSG);
	msg->type = 'H';



	retsend = send(sockfd,msg,msglen,0);
	if(-1 == retsend)
	{
		perror("fail to send.\n");
		exit(EXIT_FAILURE);
	}

	
	retrecv = recv(sockfd,msg,msglen,0);
	if(-1 == retrecv)
	{
		perror("fail to recv.\n");
		exit(EXIT_FAILURE);
	}
	printf("%sHistory:%s%s\n",GREEN,msg->history,NONE);

}



