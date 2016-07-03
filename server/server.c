#include "dict_server.h"
#include "color.h"



int main(int argc, const char *argv[])
{

	int sockfd,connfd;
	struct sockaddr_in servaddr,cliaddr;
	const char* ip_addr = "0.0.0.0";
	pid_t pid;

	size_t msglen = sizeof(MSG); 
	socklen_t len = sizeof(servaddr);
	
	sqlite3 *infodb;
	sqlite3_open("./server/info.db",&infodb);


	sqlite3 *dictdb;
	sqlite3_open("./server/dict.db",&dictdb);



	
	char infoBuf[512];
	char *infoPtr = "create table user(USERNAME char[128],PASSWORD char[128],HISTORY char[1024])";
	char *error_msg = 0;
	int ret;

	ret = sqlite3_exec(infodb,infoPtr,NULL,NULL,&error_msg);
	if(ret != SQLITE_OK)
	{
		printf("error:%s\n",error_msg);
		sqlite3_free(error_msg);
		exit(EXIT_FAILURE);
	}
	
	signal(SIGCHLD,SIG_IGN);

	myTCPSerPreconnect(&sockfd,&servaddr,ip_addr,&len);

	while(1)
	{
		myTCPSerAccept(&sockfd,&servaddr,&connfd,&len);

		pid = fork();

		if(pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if(0 == pid)
		{
			close(sockfd);
			bzero(&msg,msglen);
			bzero(&cliaddr,len);


			while(recv(connfd,&msg,msglen,0) > 0)
			{
				printf("type:%c name:%s data:%s\n",msg.type,msg.name,msg.data);
				switch (msg.type){

				case 'r':case 'R':
					do_serverregister(connfd,&msg,infodb);
					break;

				case 'l':case 'L':
					do_serverlogin(connfd,&msg,infodb);
					break;

				case 'q':case 'Q':
					do_serverquery(connfd,&msg,dictdb,infodb);
					break;

				case 'h':case 'H':
					do_serverhistory(connfd,&msg,infodb);
					break;
				default:
					break;
				}
			}
			close(connfd);
			printf("%s logout\n",msg.name);
			sqlite3_close(infodb);
			sqlite3_close(dictdb);
		}
		else
		{
			close(connfd);
		}

	}

	sqlite3_close(infodb);
	sqlite3_close(dictdb);
	return 0;
}
