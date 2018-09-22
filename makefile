# all: mTorrent share
CC = g++ 
CFLAGS = -Wall -std=c++14 -g -pthread

DEPS = ClientServer.h
OBJ =  ClientServer.o ClientConnection.o ServerConnection.o CreateSeederList.o -lcrypto
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

mTorrent: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
