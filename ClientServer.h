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
string mtorrentFile(string, string, string, string);
string CalculateSHA(string,int);
void SendToSeederList(string , string, int, string,int);

// Main Function
void tokenized(string );
void UpdateDownloadLog(int ,string );
void ShowDownloadDetail();
void ReadLogFile(string);
void UpdateLogFile(string);
void UpdateToTracker();


//Client Function
int FunctionCalling(string,int,string, string);
int clientConnection(string, int, string &, int, int, int,vector<int> BitMap1, int, ofstream &output,string);
int TrackerConnection(string, int);
void GetSeedersDetails(int, string);
void RecieveData(int, string, int);
string ReadSHA(int);
void ReadFileByLine(int, int);
void RecieveDataFromServer(int ,string ,int ,int, int , vector<int> BitMap1,int, ofstream &output, string);
void PieceSelection(int ,int ,int BitMap[]);
void BitVectorRequestToServers(string , int , int , int, string);

//Server Function
void SendData(int);
void ServerConnection();
void Communication(int server_fd,struct sockaddr_in address);
void BitVectorDetailSend(int);
void SetBitMap(int , int, string );

