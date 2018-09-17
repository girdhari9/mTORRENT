// #include "tcpConnection.h"
#include<bits/stdc++.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <sys/stat.h> 
#include <netinet/in.h> 
#include<fcntl.h>

#define PORT 9000
#define BUFFER_SIZE 2000
#define DataSize 1024
using namespace std;
   
void ServerConnection();
void Communication(int server_fd,struct sockaddr_in address);

int main(){ 
    ServerConnection();
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
            close(server_fd);
            char recvBuffer[BUFFER_SIZE];
            char sendBuffer[BUFFER_SIZE]; 
            memset(&recvBuffer, '\0',BUFFER_SIZE);
            memset(&sendBuffer, '\0',BUFFER_SIZE);

            string SrcFileName = "input.mp4";
            size_t read_size;
            struct stat fileStat;
            int err = stat(SrcFileName.c_str(), &fileStat); 
            if (0 != err){
                cout<<"Error occured in file";
                exit(0);
            } 
            int srcFilePtr = open(SrcFileName.c_str(), O_RDONLY, O_SYNC);
            
            read( new_socket , recvBuffer, 40); 
            printf("%s\n",recvBuffer );
            memset(&recvBuffer, '\0',BUFFER_SIZE);
            printf("> Sending data...\n");

            int ack;
            // send(new_socket , (char *)sendBuffer, read_size,0); // To send file name
            send(new_socket , &fileStat.st_size, sizeof(int),0);
            read(new_socket, &ack, sizeof(int));
            while((read_size = read(srcFilePtr,sendBuffer,DataSize)) > 0){
                send(new_socket , (char *)sendBuffer, read_size,0);
                read( new_socket , recvBuffer, 20);
                memset(&sendBuffer, '\0',BUFFER_SIZE);
                // printf("Packet Number: %s\n",recvBuffer );
            }
            // char temp_buffer;
            // send(new_socket , (char *)temp_buffer, 0,0);
            printf("> Data sent!\n");
            exit(0);
        }
        else{
            cout<<"[+]server busy...\n";
        }
    }
}