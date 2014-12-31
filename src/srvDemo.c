/*
 *	Server Demo of OpenSSL_Demo
 *	Date:Dec.31th,2014
 *	Author:Langley Chang
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#define PORT 22087
#define BUFFER_SIZE 1024

int main()
{
	struct sockaddr_in client_addr;
	SSL *ssl;
	SSL_CTX* ctx = init_SSL(NULL,NULL);
	int srv_fd = set_conn(PORT);
	int client_conn = accept_s(&ssl,&ctx,srv_fd,&client_addr);
	char buffer[BUFFER_SIZE];

	memset(buffer,0,sizeof(buffer));
	int len = SSL_read(ssl,buffer,sizeof(buffer));
	printf("%s\n",buffer);
	SSL_write(ssl, buffer, len);
	release(client_conn,srv_fd,ssl,ctx);
}

