#include<bits/stdc++.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <sys/stat.h> 
#include <netinet/in.h> 
#include<fcntl.h>
#include<arpa/inet.h>
#include<openssl/sha.h>
#include<thread>
#include<semaphore.h> 
#include<sys/wait.h>

#define BUFFER_SIZE 2000
#define DataSize 1024

using namespace std;

void ServerConnection();
void Communication(int server_fd,struct sockaddr_in address);

int FunctionCalling(string);
int clientConnection(string, int, string, int, int, int, int, int, ofstream &output);
int TrackerConnection(string, int);
void GetSeedersDetails(int, string);
void RecieveData(int, string, int);
string ReadSHA(int);
void ReadFileByLine(int, int);
void RecieveBitVector(int ,string ,int ,int, int, int, int, ofstream &output);
void PieceSelection(int ,int ,int BitMap[]);
void BitVectorRequestToServers(string , int , string , int );