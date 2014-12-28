/*
*	Client of OpenSSL
*	Date:Dec.27th,2014
*	Author:Langley Chang
*/

#include <stdio.h>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define PORT 22087
#define BUFFER_SIZE 2048
#define EXIT_IF_TRUE(x) if (x)							\
	do{													\
			fprintf(stderr, "Check '%s' is true\n", #x);\
			ERR_print_errors_fp(stderr);				\
			exit(2);									\
	}while(0)

int main(int argc,char *argv[])
{	
	SSL *ssl = NULL;
	SSL_CTX *ctx = NULL;
	X509 *client_cert;
	char read_buff[BUFFER_SIZE];
	char send_buff[BUFFER_SIZE]; 
	int read_length = 0;
	struct sockaddr_in addr;
	int nClientFd;
	int addr_length = 0;
	
	/* Initializing OpenSSL */
	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
	
	// using SSLv2,v3
	EXIT_IF_TRUE((ctx = SSL_CTX_new( SSLv23_client_method())) == NULL );
	
	// set normal socket connection
	nClientFd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in nClientAd;
	nClientAd.sin_family = AF_INET;
	nClientAd.sin_port = htons(PORT);
	nClientAd.sin_addr.s_addr = inet_addr(SERVERIP);
	connect(nClientFd, (struct sockaddr *)& nClientAd,sizeof(struct sockaddr));

	memset(&addr, 0, sizeof(addr));
	addr_length = sizeof(addr);

	//submit socket connection to SSL
	EXIT_IF_TRUE((ssl = SSL_new (ctx)) == NULL);
	SSL_set_fd(ssl,nClientFd);
	EXIT_IF_TRUE(SSL_connect(ssl) == -1);
	
	printf("Connect successfully.\n");

	//process the conn
	memset(read_buff, 0, sizeof(read_buff));
	memset(send_buff, 0, sizeof(send_buff));

	for(int i = 0;i < 5;i ++)
	{
		strcpy(send_buff,"Aloha");

		SSL_write(ssl, send_buff, strlen(send_buff));
		read_length = SSL_read(ssl, read_buff, sizeof(read_buff));
		fprintf(stderr, "Get Len %d %s ok\n", read_length, read_buff);
	}

	//release
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	close(nClientFd);
}
