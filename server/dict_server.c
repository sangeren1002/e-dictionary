#include "dict_server.h"
#include "color.h"

int searchflag = 0;


void myTCPSerPreconnect(int *sockfd,struct sockaddr_in *servaddr,const char *ip_addr,socklen_t *len)
{
    if(NULL == sockfd || NULL == servaddr || NULL == ip_addr || NULL == len)
    {
        printf("%ssyntax error:mind the pointer argument%s\n",RED,NONE);
        exit(EXIT_FAILURE);
    }

    int retlisten,retbind;
    int optval = 1;

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

    setsockopt(*sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

    retbind = bind(*sockfd,(struct sockaddr *)servaddr,*len);
    if(-1 == retbind)
    {
        perror("fail to bind");
        exit(EXIT_FAILURE);
    }
    printf("%sbind successfully%s\n",GREEN,NONE);

    retlisten = listen(*sockfd,BACKLOG);
    if(-1 == retlisten)
    {
        perror("fail to listen");
        exit(EXIT_FAILURE);
    }
    printf("%slisten successfully%s\n",GREEN,NONE);

}

void myTCPSerAccept(int *sockfd,struct sockaddr_in *servaddr,int *connfd,socklen_t *len)
{
    if(NULL == sockfd || NULL == servaddr || NULL == len || NULL == connfd || NULL == len )
    {
       	printf("%ssyntax error:mind the pointer argument%s\n",RED,NONE);
        exit(EXIT_FAILURE);
    }
    *connfd = accept(*sockfd,(struct sockaddr *)servaddr,len);
    if(-1 == *connfd)
    {
        perror("fail to accept");
        exit(EXIT_FAILURE);
    }
  	printf("%sconnect successfully%s\n",GREEN,NONE);
}



int callbackRegister(void *connfd,int f_num,char **f_value,char **f_name)
{
	printf("Callback record,\n");
	searchflag++;
	return 0;
}

void do_serverregister(int connfd,MSG *msg,sqlite3 *infodb)
{
	char buf[128];
	int ret;
	char *infoPtr;
	char infobuf[512];
	ssize_t retsend;
	char *error_msg = 0;
	int count ;

	count =  searchflag;

	sprintf(infobuf,"select * from user where USERNAME='%s'",msg->name);
	printf("%s\n",infobuf);

	infoPtr = infobuf;
	ret = sqlite3_exec(infodb,infoPtr,callbackRegister,NULL,&error_msg);
	if(ret != SQLITE_OK)
	{
		printf("error :%s\n",error_msg);
		sqlite3_free(error_msg);
		exit(EXIT_FAILURE);
	}
	printf("fininsh search.\n");
	
	if(count != searchflag)

	{
		strcpy(buf,"This username already exists.");
		retsend = send(connfd,buf,sizeof(buf),0);
		if(-1 == retsend)
		{
			perror("fail to send");
			exit(EXIT_FAILURE);
		}

		searchflag = 0;
	}
	else
	{
		sprintf(infobuf,"insert into user values('%s','%s','%s')",msg->name,msg->data,msg->history);
		printf("%s\n",infobuf);

		infoPtr = infobuf;
		ret = sqlite3_exec(infodb,infoPtr,NULL,NULL,&error_msg);
		if(ret != SQLITE_OK)
		{
			printf("error :%s\n",error_msg);
			sqlite3_free(error_msg);
			exit(EXIT_FAILURE);
		}

		printf("insert successfully.\n");

		//sqlite3_close(infodb);

		strcpy(buf,"Register successfully.");
		retsend = send(connfd,buf,sizeof(buf),0);
		if(-1 == retsend)
		{
			perror("fail to send");
			exit(EXIT_FAILURE);
		}
	}
}





int callbacklogin(void *Notused,int f_num,char **f_value,char **f_name)
{
	printf("'%s'  '%s'",f_value[0],f_value[1]);
	printf("callback:%c  '%s'  '%s'\n",msg.type,msg.name,msg.data);
	if((strcmp(f_value[0],msg.name) == 0) && (strcmp(f_value[1],msg.data) == 0))
	{
		printf("Login successfully.\n");
		searchflag = 1;
	}
	else
	{
		printf("The username doesn't match the password.\n");
		searchflag = 2;
	}

	return 0;
}

void do_serverlogin(int connfd,MSG *msg,sqlite3 *infodb)
{
	char buf[128];
	int ret;
	char *infoPtr;
	char infobuf[512];
	ssize_t retsend;
	char *error_msg = 0;

	printf("type:%c name:%s data:%s\n",msg->type,msg->name,msg->data);

	sprintf(infobuf,"select * from user where USERNAME='%s'",msg->name);
	printf("%s\n",infobuf);

	infoPtr = infobuf;
	
	ret = sqlite3_exec(infodb,infoPtr,callbacklogin,NULL,&error_msg);
	if(ret != SQLITE_OK)
	{
		printf("error :%s\n",error_msg);
		sqlite3_free(error_msg);	
	}

	if(searchflag == 1)
	{
		strcpy(buf,"Login successfully.");

		retsend = send(connfd,buf,sizeof(buf),0);
		if(-1 == retsend)
		{
			perror("fail to send");
			exit(EXIT_FAILURE);
		}
		searchflag = 0;

	}
	else if(searchflag == 2)
	{
		strcpy(buf,"This username does't match the password.");
		retsend = send(connfd,buf,sizeof(buf),0);
		if(-1 == retsend)
		{
			perror("fail to send");
			exit(EXIT_FAILURE);
		}
		searchflag = 0;
	}
	else
	{
		strcpy(buf,"This user does't exist");
		retsend = send(connfd,buf,sizeof(buf),0);
		if(-1 == retsend)
		{
			perror("fail to send");
			exit(EXIT_FAILURE);
		}
	}
}



int callbackquery(void *Notused,int f_num,char **f_value,char **f_name)
{

	searchflag++;
	strcat(f_value[0]," ");

	strcat(historyBuf,f_value[0]);

	strcpy(msg.word, strcat(f_value[0],f_value[1]));
	printf("%s\n",msg.data);
	
	return 0;
}


void do_serverquery(int connfd,MSG *msg,sqlite3 *dictdb,sqlite3 *infodb)
{
	char buf[128];
	int ret;
	char *dictPtr;
	char dictbuf[512];
	ssize_t retsend;
	char *error_msg = 0;
	int count;
	char *infoPtr;
	char infoBuf[512];
	count = searchflag;

	printf("type:%c name:%s data:%s word:%s\n",msg->type,msg->name,msg->data,msg->word);

	sprintf(dictbuf,"select * from dict where WORD='%s'",msg->word);
	printf("%s\n",dictbuf);

	dictPtr = dictbuf;
	
	ret = sqlite3_exec(dictdb,dictPtr,callbackquery,NULL,&error_msg);
	if(ret != SQLITE_OK)
	{
		printf("error :%s\n",error_msg);
		sqlite3_free(error_msg);	
	}

	if(count != searchflag)
	{
		retsend = send(connfd,msg,sizeof(MSG),0);
		if(-1 == retsend)
		{
			perror("fail to send");
			exit(EXIT_FAILURE);
		}

		sprintf(infoBuf,"update user set HISTORY='%s' where  USERNAME='%s'",historyBuf,msg->name);
		printf("%s\n",infoBuf);

		infoPtr = infoBuf;
		ret = sqlite3_exec(infodb,infoPtr,NULL,NULL,&error_msg);
		if(ret != SQLITE_OK)
		{
			printf("error :%s\n",error_msg);
			sqlite3_free(error_msg);	
		}

		printf("upadate successfully.\n");
	}

	else
	{
		strcpy(msg->word,"This word doesn't exist.");
		retsend = send(connfd,msg,sizeof(MSG),0);
		if(-1 == retsend)
		{
			perror("fail to send");
			exit(EXIT_FAILURE);
		}
	}
}



int callbackhistory(void *Notused,int f_num,char **f_value,char **f_name)
{
	searchflag++;
	printf("'%s'  '%s'",f_value[0],f_value[1],f_value[2]);
	strcpy(msg.history,f_value[2]);
	return 0;
}
void do_serverhistory(int connfd,MSG *msg,sqlite3 *infodb)
{
	char buf[128];
	int ret;
	char *infoPtr;
	char infobuf[512];
	ssize_t retsend;
	char *error_msg = 0;
	int msglen = sizeof(MSG);
	int count;
	count = searchflag;

	printf("type:%c name:%s data:%s\n",msg->type,msg->name,msg->data);

	sprintf(infobuf,"select * from user where USERNAME='%s'",msg->name);
	printf("%s\n",infobuf);

	infoPtr = infobuf;
	
	ret = sqlite3_exec(infodb,infoPtr,callbackhistory,NULL,&error_msg);
	if(ret != SQLITE_OK)
	{
		printf("error :%s\n",error_msg);
		sqlite3_free(error_msg);	
	}

	if(count != searchflag)
	{
		if(strcmp(msg->history," ")== 0)
		{
			strcpy(msg->history,"No history.");
			retsend = send(connfd,msg,msglen,0);
			if(-1 == retsend)
			{
				perror("fail to send");
				exit(EXIT_FAILURE);
			}

		}
		else
		{
			retsend = send(connfd,msg,msglen,0);
			if(-1 == retsend)
			{
				perror("fail to send");
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		strcpy(msg->history,"Cannot find this user.");
		retsend = send(connfd,msg,msglen,0);
		if(-1 == retsend)
		{
			perror("fail to send");
			exit(EXIT_FAILURE);
		}
	}
}
