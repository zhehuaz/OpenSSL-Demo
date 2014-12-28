
all: server.o main.o

server.o:server.c
	gcc -g  server.c -std=c99 -o server -lssl -lcrypto 
main.o:main.c
	gcc -g main.c -std=c99 -o main -lssl -lcrypto
