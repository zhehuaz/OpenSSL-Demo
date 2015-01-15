OpenSSL-Demo
============
Description
------------
This is a simple openssl connection demo.Shows main steps to set a secure connection with OpenSSL.
Only tested in Ubuntu 14.04, OpenSSL 1.0.1j.

Guide
------------

You need to download and install OpenSSL first.

https://github.com/openssl/openssl

Or ubuntu & debian use:

* $ sudo apt-get  install openssl



Server and Client are now in one folder, and you need to make them only once:

* $ cd src

* $ make

* $ make clean




In fact, the binary file already exits in this folder, maybe you can directly run them :


* $ ./server	#server

* $ ./main	#client


In the `OpenSSL-Demo/src` folder, you can see two files as utilities, `client.c` and `server.c`, which contains utility functions needed in setting SSL connection. `main.c`(for client) and `srvDemo.c`(for server) shows usage of these functions.

In client, ip is 127.0.0.1, and port is 22087 by default as you can see in the source.Besides, ip and port can also input as arguement. Yet,be careful the order...  %D



The cacert and privkey used in the program settles in 'OpenSSL-Demo/src' for demo.

If you need your own, try : 


* $ openssl genrsa -des3 -out privkey.pem 2048

* $ openssl req -new -x509 -key privkey.pem -out cacert.pem -days 365



privkey.pem and cacert.pem will exit in your folder. More infomation about SSL see at google.
 



More
------------


I keep working on this project and about to update some simple applications with OpenSSL.


Still, there's some problems and bugs.


See `OpenSSL-Demo/doc` for details.









