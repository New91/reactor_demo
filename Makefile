server: server.cpp
	g++  $^ -o $@ 

clean:
	rm -f *.o
	rm -f server