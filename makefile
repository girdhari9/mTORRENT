all: miniTorrent share
CC = g++ 
CFLAGS = -Wall -std=c++14 -g -pthread

DEPS = ClientServer.h
OBJ =  ClientServer.o ClientConnection.o ServerConnection.o 
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

miniTorrent: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

CC = g++ 
CFLAG = -Wall -std=c++14 -g

DEPS = mtorrent.h
OBJ =  mtorrent.o CreateTorrentFile.o -lcrypto
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAG) -c -o $@ $<

share: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^