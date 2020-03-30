server: server.cpp do_it.cpp
	g++  $^ -o $@ 

clean:
	rm -f *.o
	rm -f server