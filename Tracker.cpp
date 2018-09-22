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
void Communication(int ,struct sockaddr_in);
void ReadSeedersDetail(int ,int );

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
        int RequestType;
        read(new_socket , &RequestType, sizeof(int));
        if(RequestType == 1){
            if(fork() == 0){
                char recvBuffer[BUFFER_SIZE];
                memset(&recvBuffer, '\0',BUFFER_SIZE);
                map<string,string> SeederListMap;

                printf("> You are adding in seederlist...\n");

                int read_size, index = 0, ack = 2,flag = 0, port, check = 1;
                int RootPathSize;
                read(new_socket , recvBuffer, 20);

                string SHA = (string)recvBuffer;
                char RootPath[1000];
                memset(&recvBuffer, '\0',1000);

                read(new_socket, &read_size, sizeof(int));
                read(new_socket, recvBuffer, read_size);
                read(new_socket, &port, sizeof(int));
                read(new_socket, &RootPathSize, sizeof(int));
                read(new_socket, RootPath, RootPathSize);

                string NewSeeder = (string)recvBuffer + " " + to_string(port) + " " + (string)RootPath;

                ifstream TrackerFileD("trackerfile.txt", ifstream::binary);
                string Seeders,shaline;
                if(TrackerFileD.is_open()){
                    while (getline(TrackerFileD,shaline)) {
                        getline(TrackerFileD,Seeders);
                        if(check && shaline == SHA){
                            Seeders = Seeders + " " + NewSeeder;
                            check = 0;
                            SeederListMap[shaline] = Seeders;
                        }
                        else
                            SeederListMap[shaline] = Seeders;
                    }
                    if(check) SeederListMap[SHA] = NewSeeder;
                    TrackerFileD.close();
                }
                else{
                    cout<<"Error: Please Try to connect after some time\n";
                }
                ofstream TrackerFile("trackerfile.txt", ofstream::binary);
                if(TrackerFile.is_open()){
                    for(auto it = SeederListMap.begin(); it != SeederListMap.end(); it++){
                        TrackerFile << (it->first).c_str();
                        TrackerFile << "\n";
                        TrackerFile << (it->second).c_str();
                        TrackerFile << "\n";
                    }
                    TrackerFile.close();
                }
                else{
                    cout<<"Unable to add you in seeder list\n";
                }
                close(new_socket);
                exit(0);
            }
            else{
                cout<<"[+]server busy...\n";
            }
        }
        else{
            if(fork() == 0){
                char recvBuffer[BUFFER_SIZE];
                memset(&recvBuffer, '\0',BUFFER_SIZE);

                printf("> Seeders Detail Sending...\n");
                read(new_socket , recvBuffer, 20);

                ifstream TrackerFileD("trackerfile.txt", ifstream::binary);

                string Seeders,shaline;
                if(TrackerFileD.is_open()){
                    while (getline(TrackerFileD,shaline)) {
                        getline(TrackerFileD,Seeders);
                        if(shaline == (string)recvBuffer){
                            int TotalSeeders = Seeders.size();
                            send(new_socket , &TotalSeeders , sizeof(int),0);
                            send(new_socket , Seeders.c_str(), TotalSeeders,0);
                            cout<<"> Seeders details sent!\n";
                            break;
                        }
                    }
                    TrackerFileD.close();
                }
                else{
                    cout<<"Error: Please Try to connect after some time\n";
                }
                close(new_socket);
                exit(0);
            }
            else{
                cout<<"[+]server busy...\n";
            }
        }
    }
}
