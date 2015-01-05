/*
 *	Server Demo of OpenSSL
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

/*
 * In the main function,you need to define SSL and SSL_CTX(SSL context).
 * SSL_CTX describes environment of your socket connction with certificate and private key.And only '.pem' file supported.
 * About generating the certificate and private key,see at README.
 * */
int main()
{
	struct sockaddr_in client_addr;//this is to store client infomation
	SSL *ssl;//stands for a client SSL connection like client_fd in normal socket

	//using default path as good as init_SSL("cert.pem","privkey.pem") 
	SSL_CTX* ctx = SSL_init(NULL,NULL);
	
	//listen to a port
	int srv_fd = SSL_set_srv(PORT);
	//waiting for client
	int client_fd = SSL_waiting(&ssl,&ctx,srv_fd,&client_addr);
	char buffer[BUFFER_SIZE];
	memset(buffer,0,sizeof(buffer));

	//using SSL_read()
	int len = SSL_read(ssl,buffer,sizeof(buffer));
	printf("%s\n",buffer);
	
	//using SSL_write()
	SSL_write(ssl, buffer, len);
	release(client_fd,srv_fd,ssl,ctx);
}

