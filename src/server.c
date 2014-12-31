/*
*	Sever of socket_Lab
*	Date:Dec 31th,2014
*	Author:Langley Chang
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#define QUEUE   20
#define EXIT_IF_TRUE(x) if(x)						\
	do{												\
		fprintf(stderr, "Check '%s' is true\n", #x);\
		ERR_print_errors_fp(stderr);				\
		exit(2);									\
	}while(0)

SSL_CTX* init_SSL(const char* cert_path,const char* privkey_path)
{
	SSL_CTX *ctx = NULL;
	X509 *client_cert;
	
	/* Initializing OpenSSL */
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	if(cert_path == NULL)
		cert_path = "cacert.pem";
	if(privkey_path == NULL)
		privkey_path = "privkey.pem";

	// using SSLv2,v3
	EXIT_IF_TRUE((ctx = SSL_CTX_new( SSLv23_server_method())) == NULL);
	// load my certs
	EXIT_IF_TRUE(SSL_CTX_use_certificate_file(ctx, cert_path,SSL_FILETYPE_PEM) <= 0);
	// load my private key
	EXIT_IF_TRUE(SSL_CTX_use_PrivateKey_file(ctx, privkey_path,SSL_FILETYPE_PEM) <= 0);
	// check if private key is correct
	EXIT_IF_TRUE(!SSL_CTX_check_private_key(ctx));

	return ctx;
}

//bind & listen
int set_conn(const int port) 
{
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    EXIT_IF_TRUE(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1);
    EXIT_IF_TRUE(listen(server_sockfd,QUEUE) == -1);

	return server_sockfd;
}

//accept
int accept_s(SSL **ssl,SSL_CTX **ctx,const int server_sockfd,struct sockadd_in* client_addr)
{
    socklen_t length = sizeof(client_addr);
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    EXIT_IF_TRUE(conn<0);

	//submit socket connection to SSL
	EXIT_IF_TRUE(((*ssl) = SSL_new(*ctx)) == NULL);
	SSL_set_fd((*ssl),conn);
	EXIT_IF_TRUE(SSL_accept(*ssl) != 1);

	return conn;
}

//close
int release(int client_sockfd,int server_sockfd,SSL* ssl,SSL_CTX* ctx)
{
    close(client_sockfd);
    close(server_sockfd);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
}	

