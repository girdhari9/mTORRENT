CC = g++ 
CFLAGS = -Wall -std=c++14 -g 
DEPS = mtorrent.h
OBJ =  mtorrent.o CreateTorrentFile.o -lcrypto
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

mTorrent: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

