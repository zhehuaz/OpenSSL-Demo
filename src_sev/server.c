/*
*	Sever of socket_Lab
*	Date:Dec.27th,2014
*	Author:Langley Chang
*/

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#define MYPORT  22087
#define QUEUE   20
#define BUFFER_SIZE 1024
#define EXIT_IF_TRUE(x) if(x)						\
	do{												\
		fprintf(stderr, "Check '%s' is true\n", #x);\
		ERR_print_errors_fp(stderr);				\
		exit(2);									\
	}while(0)

int main()
{
	SSL *ssl = NULL;
	SSL_CTX *ctx = NULL;
	X509 *client_cert;
	
	/* Initializing OpenSSL */
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();

	// using SSLv2,v3
	EXIT_IF_TRUE((ctx = SSL_CTX_new( SSLv23_server_method())) == NULL);
	// load my certs
	EXIT_IF_TRUE(SSL_CTX_use_certificate_file(ctx, "cacert.pem",SSL_FILETYPE_PEM) <= 0);
	// load my private key
	EXIT_IF_TRUE(SSL_CTX_use_PrivateKey_file(ctx,"privkey.pem",SSL_FILETYPE_PEM) <= 0);
	// check if private key is correct
	EXIT_IF_TRUE(!SSL_CTX_check_private_key(ctx));

	// set normal scoket connection
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    EXIT_IF_TRUE(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1);
	printf("Waiting...\n");

    EXIT_IF_TRUE(listen(server_sockfd,QUEUE) == -1);
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0)
    {
        perror("connect ERROR");
        exit(1);
    }	
	else
		printf("User Connected.\n");

	//submit socket connection to SSL
	EXIT_IF_TRUE((ssl = SSL_new(ctx)) == NULL);
	SSL_set_fd(ssl,conn);
	EXIT_IF_TRUE(SSL_accept(ssl) != 1);

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int len = SSL_read(ssl, buffer, sizeof(buffer));
        if(!strcmp(buffer,"exit\n") || len == 0)
            break;
		printf("%s",buffer);
        SSL_write(ssl, buffer, len);
    }
	printf("User Exit.\n");
    close(conn);
    close(server_sockfd);
	
    return 0;
}
