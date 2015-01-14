# OpenSSL-Demo makefile for server and client by Zhehua Chang
# Lastest edit: Jan 14th 

SRVBIN = srvDemo
CLIBIN = main
SRVUTIL = server
CLIUTIL = client
OPENSSLLIB = -lssl -lcrypto

all: $(SRVBIN) $(CLIBIN) 

#compile server
$(SRVBIN):$(SRVBIN).o $(SRVUTIL).o
	gcc -g -o $(SRVBIN) $(SRVBIN).o $(SRVUTIL).o $(OPENSSLLIB)
$(SRVBIN).o:$(SRVUTIL).o
	gcc -c -g $(SRVBIN).c -std=c99 -o $(SRVBIN).o $(OPENSSLLIB)
$(SRVUTIL).o:
	gcc -c -g $(SRVUTIL).c -std=c99 -o $(SRVUTIL).o $(OPENSSLLIB)


#compile client
$(CLIBIN):$(CLIUTIL).o $(CLIBIN).o
	gcc -g -o $(CLIBIN) $(CLIBIN).o $(CLIUTIL).o $(OPENSSLLIB)
$(CLIBIN).o:$(CLIUTIL).o
	gcc -c -g $(CLIBIN).c -o $(CLIBIN).o $(OPENSSLLIB)
client.o:
	gcc -c -g $(CLIUTIL).c -o $(CLIUTIL).o $(OPENSSLLIB) 

#clean command
clean:
	rm *.o
