#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

int callback(void *para, int f_num, char **f_value, char **f_name)
{
	printf("HELLO CALL BACK!!!!!!\n");
	//1.para  外部给callback传参
	//2.f_num 结果输出的 行 乘 列
	//3. fvalue   结果对应的内容
	//4. fname    结果对应的表头（属性）
	//   GUOPENG  NAME
	//   WOMAN    SEX
	//   WULAOSHI  NAME
	//   MAN      SEX
	int i=0;
	for(i=0;i<f_num;i++)
	{
	
		printf("%s \t %s: ",f_name[i], f_value[i]);
	
	}
	printf("\n");
	return 0;
}


void split(char *lineBuf,char *dictBuf)
{
	char *p;
	char *q;

	char wordChunks[64];
	char explanationChunks[512];


	q  = wordChunks;
	for(p = lineBuf;*p != ' ';)
	{
		if(*p == 39)
		{
			*q++ = 39;
			*q = 39;
			p++;
			q++;
		}
		*q++ = *p++;
	}
	*q = '\0';

	while(*p == ' ')
	{

		p++;
	}

	q = explanationChunks;
	while(*p != '\n')
	{
		if(*p == 39)
		{
			*q++ = 39;
			*q = 39;
			p++;
			q++;
		}
		else
		{
			*q++ = *p++;
		}
	}
	*(q-1) = '\0';

	sprintf(dictBuf,"insert into dict values('%s','%s')",wordChunks,explanationChunks);
	//printf("%s\n",dictBuf);
	
}

int main(int argc, const char *argv[])
{


	FILE * fp = fopen(argv[1],"r");
	if(NULL == fp)
	{
		printf("fail to open dict.txt.\n");
		exit(EXIT_FAILURE);
	}
	printf("open successfully.\n");


	char lineBuf[1024];
	char dictBuf[1024];
	char *linePtr = lineBuf;
	ssize_t len = 1024;
	ssize_t linelen;
	int linenum = 0;

	sqlite3 *db;
	sqlite3_open(argv[2],&db);


	char *dictPtr="create table dict (WORD char[50],EXPLANATION  char[512])";
	char *error_msg=0;
	int ret;

	ret=sqlite3_exec(db,dictPtr,NULL,NULL,&error_msg);
	if(ret!=SQLITE_OK)
	{
		printf("error :%s\n", error_msg);
		sqlite3_free(error_msg);
		exit(EXIT_FAILURE);
	}


	while(feof(fp) == 0)
	{
		linelen = getline(&linePtr,&len,fp);

		linenum ++;
		printf("linelen = %d linenum = %d\n",linelen,linenum);
		if(0 == linelen)
		{
			printf("fail to read file data.\n");
		}

		if(linelen < 0)
			break;

		split(lineBuf,dictBuf);

		dictPtr = dictBuf;
		ret=sqlite3_exec(db,dictPtr,NULL,NULL,&error_msg);
		if(ret!=SQLITE_OK)
		{
			printf("error :%s\n", error_msg);
			sqlite3_free(error_msg);
			exit(EXIT_FAILURE);
		}

		bzero(dictBuf,256);
		bzero(lineBuf,256);

	}
#if 0
	dictPtr = "select * from dict";
	ret=sqlite3_exec(db,dictPtr,callback,NULL,&error_msg);
	if(ret!=SQLITE_OK)
	{
		printf("error :%s\n", error_msg);
		sqlite3_free(error_msg);

	}
#endif
	fclose(fp);
	sqlite3_close(db);
	return 0;
}
