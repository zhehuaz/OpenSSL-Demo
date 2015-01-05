/*
*	Server Utils of OpenSSL 
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


/*
 * This file contains main steps to set an SSL connection in server
 * SSL is a protocal between transmission layer and application layer
 * It means you need only set a normal socket connection in TCP or UDP,then submit it to SSL
 * That's all
 * In this file,TCP and IPv4 are used for socket,and SSLv23 for SSL.Feel free to use others for your condition
 * 'bio.h' defines BIO structure.This is a very useful IO type,containing plenty of types of IO,including stdIO,socket and so on
 * Bound by my power,I didn't use it and set a normal socket
 * But I strongly recommend you to try it.And if possible,pull your update to my repo,I'll appreciate it
 * */




/*
 * Input certificate and private key path to init the context of ssl
 *
 * if Input is NULL,default file would be used.
 * */
SSL_CTX* SSL_init(const char* cert_path,const char* privkey_path)
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

// bind & listen
int SSL_set_srv(const int port) 
{
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;// IPv4
    server_sockaddr.sin_port = htons(port);// set port
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);// listen any addresses
    EXIT_IF_TRUE(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1);// bind
    EXIT_IF_TRUE(listen(server_sockfd,QUEUE) == -1);// listen

	return server_sockfd;
}

/* As good as accept()
 * The funtion 'SSL_accept()' exits already in the 'ssl.h',so I've got to name as SSL_waiting(),containing normal accept()
 * Be careful SSL and SSL_CTX are double pointers here
 * when a client connected,the connection is submitted to SSL varible,so the ssl is as good as client_fd(conn) before
 * the client_fd is returned for further use
 * */
int SSL_waiting(SSL **ssl,SSL_CTX **ctx,const int server_sockfd,struct sockadd_in* client_addr)
{
    socklen_t length = sizeof(client_addr);
    int client_fd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    EXIT_IF_TRUE(client_fd<0);

	// submit socket connection to SSL
	EXIT_IF_TRUE(((*ssl) = SSL_new(*ctx)) == NULL);
	SSL_set_fd((*ssl),client_fd);
	EXIT_IF_TRUE(SSL_accept(*ssl) != 1);

	return client_fd;
}

/*
 * receive() and send() is not defined.
 * Just use SSL_read() and SSL_write() instead.
 * */

// close
int release(int client_sockfd,int server_sockfd,SSL* ssl,SSL_CTX* ctx)
{
    close(client_sockfd);
    close(server_sockfd);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
}	

