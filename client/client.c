#include "dict_client.h"
#include "color.h"

int main(int argc, const char *argv[])
{
	MSG msg;
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;
	int cmd;
	size_t msglen = sizeof(msg);
	int retlogin;
	int retscanf;

	socklen_t len = sizeof(servaddr);
	char ip_addr[20];
	printf("Please input the server ip:");
	fgets(ip_addr,20,stdin);

	myTCPCliPreconnect(&sockfd,&servaddr,&cliaddr,ip_addr,&len);

	myTCPCliConnect(&sockfd,&servaddr,len);


	printf("%s/***********************************/%s\n",YELLOW,NONE);
	printf("%s****1.register  2.login  3.quit****** %s\n",YELLOW,NONE);
	printf("%s/***********************************/%s\n",YELLOW,NONE);


	while(1)
	{
		printf("please input the cmd:");
		retscanf = scanf("%d",&cmd);
		getchar();

		printf("%d",retscanf);
		if(!(cmd == 1 || cmd == 2 || cmd == 3) || !retscanf)
		{
			printf("Incalid cmd.\n");
			continue;
		}

		switch(cmd){
		case 1:
		    do_clientregister(sockfd,&msg);
			break;
		case 2:
			retlogin = do_clientlogin(sockfd,&msg);
			if(0 == retlogin)
			{

				printf("%s/****************************************/%s\n",SKYBLUE,NONE);
				printf("%s***1.query_word 2.history_record 3.quit** %s\n",SKYBLUE,NONE);
				printf("%s/****************************************/%s\n",SKYBLUE,NONE);

				while(1)
				{

					printf("please input the cmd:");
					retscanf = scanf("%d",&cmd);
					getchar();

					if(!(cmd == 1 || cmd == 2 || cmd == 3) || !retscanf)
					{
						printf("Incalid cmd.\n");
						continue;
					}

					switch (cmd){
					case 1:
						do_clientquery(sockfd,&msg);
						break;

					case 2:
						do_clienthistory(sockfd,&msg);

					case 3:
						close(sockfd);
						exit(EXIT_SUCCESS);

					default:
						break;
					}

				}

			}

			break;

		case 3:
			close(sockfd);
			exit(EXIT_SUCCESS);
		default:break;
		}
	}
	return 0;
}

