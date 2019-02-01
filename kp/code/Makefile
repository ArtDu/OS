all: client server
client:
	c++ -std=c++11 -fpermissive -lpthread -pthread -Wall -D_POSIX_SOURCE=600 -D_XOPEN_SOURCE=600 client.cpp -o client -L/usr/local/lib -lzmq
server:
	c++ -std=c++11  -Wall -D_POSIX_SOURCE=600 server.cpp -o server -L/usr/local/lib -lzmq
clear:
	rm server client
