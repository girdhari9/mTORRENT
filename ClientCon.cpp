// #include "tcpConnection.h"
#include<bits/stdc++.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include<fcntl.h>

using namespace std;
#define PORT 9000 
#define DataSize 1024
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
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){ 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    memset(&recvBuffer, '\0',BUFFER_SIZE);
    memset(&sendBuffer, '\0',BUFFER_SIZE);

    cout<<"[+]system started...\n"; 
    cout<<"[+]system connecting...\n";
    string recvFileName = "recvdata.mp4";
    int recvFilePtr = open(recvFileName.c_str(), O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);;
    size_t read_size;
    long long int count = 1;
    char start_msg[] = "client is ready...\n";
    send(sock , start_msg , strlen(start_msg) , 0 );
    cout<<"[+]system connected...\n";
    printf("> recieving data...\n");
    while((read_size = read( sock , recvBuffer, DataSize)) > 0){
        char data_packet[20]; 
        cout<<"$";
        sprintf( data_packet, "%lld", count );
        // puts( data_packet );
        write(recvFilePtr,recvBuffer,read_size);
        send(sock , data_packet , strlen(data_packet) , 0 );
        count++;
    }
    close(recvFilePtr);
    printf("> Data Received!\n");
    return 0; 
} 