CXX=gcc

all:
	$(CXX) client.c -o client.out 
	$(CXX) server.c -o server.out 