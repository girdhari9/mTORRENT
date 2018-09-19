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

#define BUFFER_SIZE 2000
#define DataSize 1024

using namespace std;

void ServerConnection();
void Communication(int server_fd,struct sockaddr_in address);

int FunctionCalling(int , string);
int clientConnection(string, int);
void GetSeedersDetails(int, string);
void RecieveData(int, string, int);
string ReadSHA(int);
void ReadFileByLine(int, int);