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
#define BUFFER_SIZE 1024

int main(int argc,char *argv[])
{	
	struct sockaddr_in addr;
	SSL *ssl = NULL;
	SSL_CTX *ctx = init_SSL();
	int client_fd = set_conn(&ssl,&ctx,SERVERIP,PORT);

	char buffer[BUFFER_SIZE];
	sprintf(buffer,"Aloha");
	SSL_write(ssl, buffer, strlen(buffer));
	SSL_read(ssl, buffer, BUFFER_SIZE);
	fprintf(stdout, "%s\n", buffer);

	release(ssl,ctx,client_fd);
}
