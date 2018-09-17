// #include "tcpConnection.h"
#include<bits/stdc++.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include<fcntl.h>

using namespace std;
#define PORT 4040
#define DataSize 1024
#define BUFFER_SIZE 2000

int clientConnection(string);
void SendData(int);

int main(int arg,char * args[]){ 
    int sock = clientConnection(args[1]);
    SendData(sock);
    return 0;
} 

int clientConnection(string IPaddress){
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 

    if(inet_pton(AF_INET, IPaddress.c_str(), &serv_addr.sin_addr)<=0){ 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    return sock;
}

void SendData(int sock){
    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE]; 
    memset(&recvBuffer, '\0',BUFFER_SIZE);
    memset(&sendBuffer, '\0',BUFFER_SIZE);

    cout<<"[+]system started...\n"; 
    cout<<"[+]system connecting...\n";

    cout<<"[+]system connected...\n";
    printf("> recieving data...\n");
    string s = "abcde"; // Send data after reading from file
    int ack, DataLength;
    send(sock, s.c_str(), strlen(s.c_str()), 0 );
    read(sock, &ack, sizeof(int));

    read(sock, &DataLength, sizeof(int));
    send(sock, &ack, sizeof(int),0);
    read(sock, recvBuffer, DataLength);
    close(sock);

    int index = 0;
    string ip_add, port_add;
    while(recvBuffer[index] != ' '){
        ip_add += recvBuffer[index];
        index++;
    }
    while(recvBuffer[index] != ' '){
        port_add += recvBuffer[index];
        index++;
    }
    sock = clientConnection(ip_add);
    cout<<sock<<" "<<"Connection Done!";

    //recieve file
    printf("> Connection closed!\n");
}