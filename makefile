message: communicate/message/message.cpp
	g++ -c communicate/message/message.cpp -o message.o

client: communicate/client.cpp message
	g++ -c communicate/client.cpp message.o -o client.o

host : communicate/host.cpp message
	g++ -c communicate/host.cpp message.o -o host.o

main_client : message client
	g++ main_client.cpp message.o client.o -o main_client -lpthread

main_host : message host
	g++ main_host.cpp message.o host.o -o main_host -lpthread

all: message client host
	g++ main_host.cpp message.o host.o -o main_host -lpthread
	g++ main_client.cpp message.o client.o -o main_client -lpthread

clean_host:
	rm -f message.o
	rm -f host.o
	rm -f main_host

clean_client:
	rm -f message.o
	rm -f client.o
	rm -f main_client
