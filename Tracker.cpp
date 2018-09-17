// #include "tcpConnection.h"
#include<bits/stdc++.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <sys/stat.h> 
#include <netinet/in.h> 
#include<fcntl.h>

#define PORT 4040
#define BUFFER_SIZE 2000
#define DataSize 1024
using namespace std;

void TrackerConnection();
void Communication(int,struct sockaddr_in);

int main(){ 
    TrackerConnection();
    return 0; 
} 

void TrackerConnection(){
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
	printf("[+]server started...\n");
    int new_socket,addrlen = sizeof(address);
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

            printf("> Sending data...\n");
            int read_size, index = 0, ack = 1,flag = 0;
            read(new_socket , recvBuffer, 5);
            send(new_socket , &ack , sizeof(int),0);

            int TrackerFileD = open("trackerfile.txt", O_RDONLY, O_SYNC);
            while (read(TrackerFileD, &sendBuffer[index], 1) == 1) {
			    if (sendBuffer[index] == '\n' || sendBuffer[index] == 0x0) {
			    	string sb = (string)sendBuffer;
			    	string rb = (string)recvBuffer;
			    	if(sb.substr(0,sb.size()-1) == rb){
			    		flag = 1; index = 0;
			    		memset(&sendBuffer, '\0',BUFFER_SIZE);
			    		while (read(TrackerFileD, &sendBuffer[index], 1) == 1){
			    			if (sendBuffer[index] == '\n' || sendBuffer[index] == 0x0){
			    				send(new_socket , &index , sizeof(int),0);
			    				read(new_socket , &ack, sizeof(int));
			    				send(new_socket , (char *)sendBuffer, index,0);
			    				memset(&sendBuffer, '\0',BUFFER_SIZE);
			    				exit(0);
			    			}
			    			index++;
			    		}
			    	}
			    	memset(&sendBuffer, '\0',BUFFER_SIZE);
			    	index = 0; continue;
			    }
			    index++;
			}
            exit(0);
        }
        else{
            cout<<"[+]server busy...\n";
        }
    }
}