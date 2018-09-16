// #include "tcpConnection.h"
#include<bits/stdc++.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>

using namespace std;
#define PORT 9000 
#define BUFFER_SIZE 2000

int main() 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr;

    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE]; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    memset(&recvBuffer, '\0',BUFFER_SIZE);
    memset(&sendBuffer, '\0',BUFFER_SIZE);
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    // send(sock , sendBuffer , strlen(sendBuffer) , 0 ); 
    printf("Client open!\n"); 

    FILE * recvFilePtr;
    string recvFileName = "recvfile.txt";
    recvFilePtr = fopen(recvFileName.c_str(),"w");
    valread = read( sock , recvBuffer, 1024); 
    // fwrite(recvBuffer,1,sizeof(recvBuffer),recvFilePtr);
    fprintf(recvFilePtr, "%s\n", recvBuffer);
    printf("%s\n",recvBuffer ); 
    return 0; 
} 