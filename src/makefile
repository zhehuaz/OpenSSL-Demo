all: srvDemo main

#compile server
srvDemo:srvDemo.o server.o
	gcc -o srvDemo srvDemo.o server.o -lssl -lcrypto
srvDemo.o:server.o
	gcc -c -g srvDemo.c -std=c99 -o srvDemo.o -lssl -lcrypto
server.o:
	gcc -c -g server.c -std=c99 -o server.o -lssl -lcrypto 
clean:
	rm *.o

#compile client
main:
	gcc -g main.c -std=c99 -o main -lssl -lcrypto
