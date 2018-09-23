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
#include <iomanip>
#include <functional>
#include <new>

#define BUFFER_SIZE 2000
#define DataSize 1024
#define FILE_SIZE 524288
extern int PORT;

using namespace std;

//CreateTorrentFile
string mtorrentFile(string,string);
string CalculateSHA(string,int);
void SendToSeederList(string , string, int, string,int);

void tokenized(string );

//Client Function
int FunctionCalling(string,int,string, string);
int clientConnection(string, int, string &, int, int, int,vector<int> BitMap1, int, ofstream &output);
int TrackerConnection(string, int);
void GetSeedersDetails(int, string);
void RecieveData(int, string, int);
string ReadSHA(int);
void ReadFileByLine(int, int);
void RecieveBitVector(int ,string ,int ,int, int , vector<int> BitMap1,int, ofstream &output);
void PieceSelection(int ,int ,int BitMap[]);
void BitVectorRequestToServers(string , int , int , int);

//Server Function
void SendData(int);
void ServerConnection(string, int);
void Communication(int server_fd,struct sockaddr_in address, string, int);
void BitVectorDetailSend(int, string, int);

