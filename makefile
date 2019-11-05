CXX=gcc

all:
	$(CXX) client.c -o client.out 
	$(CXX) proxy.c -o proxy.out
	$(CXX) server-fifo.c -o fifo.out
	$(CXX) server-MC.c -o mc.out -pthread
	$(CXX) server-pipe.c -o pipe.out
