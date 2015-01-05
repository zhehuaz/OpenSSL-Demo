all: srvDemo main

#compile server
srvDemo:srvDemo.o server.o
	gcc -g -o srvDemo srvDemo.o server.o -lssl -lcrypto
srvDemo.o:server.o
	gcc -c -g srvDemo.c -std=c99 -o srvDemo.o -lssl -lcrypto
server.o:
	gcc -c -g server.c -std=c99 -o server.o -lssl -lcrypto 


#compile client
main:client.o main.o
	gcc -g -o main main.o client.o -lssl -lcrypto
main.o:client.o
	gcc -c -g main.c -o main.o -lssl -lcrypto
client.o:
	gcc -c -g client.c -o client.o -lssl -lcrypto

#clean command
clean:
	rm *.o
