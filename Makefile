all: server client

server: server.cpp do_it.cpp
	g++  $^ -o $@ 
client: client.cpp
	g++  $^ -o $@ 

clean:
	rm -f *.o
	rm -f server client