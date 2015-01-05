/*
*	Client Utils of OpenSSL
*	Date:Dec.31th,2014
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

#define BUFFER_SIZE 2048
#define EXIT_IF_TRUE(x) if (x)							\
	do{													\
			fprintf(stderr, "Check '%s' is true\n", #x);\
			ERR_print_errors_fp(stderr);				\
			exit(2);									\
	}while(0)

	
//init the SSL and return SSL_CTX
SSL_CTX* init_SSL()
{
	SSL_CTX* ctx;
	X509 *client_cert;
	// Initializing OpenSSL 
	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
	
	// using SSLv2,v3
	EXIT_IF_TRUE((ctx = SSL_CTX_new( SSLv23_client_method())) == NULL );
	return ctx;	
}

//set ip and port and try to connect the server. 
int SSL_conn(SSL **ssl,SSL_CTX **ctx,char *ip,int port)
{
	// set normal socket connection
	struct sockaddr_in addr;
	int nClientFd = socket(AF_INET,SOCK_STREAM,0);
	int addr_length = 0;
	struct sockaddr_in nClientAd;
	nClientAd.sin_family = AF_INET;//IPv4
	nClientAd.sin_port = htons(port);//port
	nClientAd.sin_addr.s_addr = inet_addr(ip);
	EXIT_IF_TRUE(connect(nClientFd, (struct sockaddr *)& nClientAd,sizeof(struct sockaddr)));

	memset(&addr, 0, sizeof(addr));
	addr_length = sizeof(addr);

	//submit socket connection to SSL
	EXIT_IF_TRUE(((*ssl) = SSL_new (*ctx)) == NULL);
	SSL_set_fd(*ssl,nClientFd);
	EXIT_IF_TRUE(SSL_connect(*ssl) == -1);

	return nClientFd;
}

void SSL_release(SSL *ssl,SSL_CTX *ctx,int client_fd)
{
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	close(client_fd);
}
