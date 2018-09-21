// #include "tcpConnection.h"
#include<bits/stdc++.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <sys/stat.h> 
#include <netinet/in.h> 
#include<fcntl.h>
#include<arpa/inet.h>
#include<semaphore.h> 
#include<sys/wait.h>

#define PORT 6000
#define BUFFER_SIZE 2000
#define DataSize 1024
using namespace std;

sem_t mutex1;
void ServerConnection();
void Communication(int server_fd,struct sockaddr_in address);

int main(){ 
    sem_init(&mutex1, 0, 1);
    ServerConnection();
    sem_destroy(&mutex1);
    return 0; 
} 

void ServerConnection(){
    int server_fd; 
    struct sockaddr_in address; 
    int opt = 1; 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0){ 
        perror("listen"); 
        exit(EXIT_FAILURE); printf("[+]server started...\n");
    }
    Communication(server_fd,address);
}

void Communication(int server_fd,struct sockaddr_in address){

    int new_socket,addrlen = sizeof(address);
    printf("[+]server started...\n");
    while(1){
        printf("[+]Listening...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){ 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 
        
        if(fork() == 0){
            char recvBuffer[BUFFER_SIZE];
            char sendBuffer[BUFFER_SIZE]; 
            memset(&recvBuffer, '\0',BUFFER_SIZE);
            memset(&sendBuffer, '\0',BUFFER_SIZE);

            size_t read_size;
            int ack, DataPackNumber;
            printf("Client is ready...\n");

            int fileNameLength,LastPartSize = 0,LoopLimit, TotalConn;

            read(new_socket, &TotalConn, sizeof(int));
            send(new_socket, &ack, sizeof(int), 0);
            read(new_socket, &LoopLimit, sizeof(int));
            send(new_socket, &ack, sizeof(int), 0);
            read(new_socket, &LastPartSize, sizeof(int));
            send(new_socket, &ack, sizeof(int), 0);
            read(new_socket, &fileNameLength, sizeof(int));
            send(new_socket, &ack, sizeof(int), 0);
            read(new_socket, recvBuffer, fileNameLength-1);
            send(new_socket, &ack, sizeof(int), 0);

            ifstream input;
            input.open(recvBuffer,ifstream::binary);
            memset(&recvBuffer, '\0',BUFFER_SIZE);

            printf("> Sending data...\n");

            int thread_n;
            read(new_socket, &thread_n, sizeof(int));
            send(new_socket, &ack, sizeof(int), 0);
            read(new_socket, &ack, sizeof(int));
            
            int index = thread_n * DataSize, LoopValue=0; 
            while(LoopValue < LoopLimit){
                input.seekg (index);
                input.read(sendBuffer,DataSize);

                send(new_socket , (char *)sendBuffer, DataSize,0);
                read( new_socket , &DataPackNumber, sizeof(int));
                memset(&sendBuffer, '\0',BUFFER_SIZE);
                LoopValue++;

                index += TotalConn * DataSize;
                
            }
            int ExtraLoopForThread = 0;
            read(new_socket, &ExtraLoopForThread, sizeof(int));
            send(new_socket, &ack, sizeof(int), 0);

            LoopValue = 0;
            while(LoopValue < ExtraLoopForThread){
                if(thread_n == ExtraLoopForThread-1){
                    input.seekg (index);
                    if(LastPartSize)
                        input.read(sendBuffer,LastPartSize);
                    else input.read(sendBuffer,DataSize);

                    send(new_socket , (char *)sendBuffer, LastPartSize,0);
                    memset(&sendBuffer, '\0',BUFFER_SIZE);
                    index += TotalConn * DataSize;
                }
                else{
                    input.seekg (index);
                    input.read(sendBuffer,DataSize);

                    send(new_socket , (char *)sendBuffer, LastPartSize,0);
                    memset(&sendBuffer, '\0',BUFFER_SIZE);
                    index += TotalConn * DataSize;
                }
                read(new_socket , &DataPackNumber, sizeof(int));
                LoopValue++;
            }
            printf("> Data sent!\n");
            close(new_socket);
            exit(0);
        }
        else{
            cout<<"[+]server busy...\n";
        }
    }
}