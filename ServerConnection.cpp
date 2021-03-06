#include "ClientServer.h"

using namespace std;

extern int PORT;
extern map<string,string> TotalFile;

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
    return ;
}

void Communication(int server_fd,struct sockaddr_in address){
    int new_socket,addrlen = sizeof(address), RequestType;
    printf("[+]server started...\n");
    while(1){
        printf("[+]Listening...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){ 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 
        read(new_socket, &RequestType, sizeof(int));
        if(RequestType == 1)
            BitVectorDetailSend(new_socket);
        else
            SendData(new_socket);
    }
}

void BitVectorDetailSend(int new_socket){
    if(fork() == 0){
        int ack, TotalPacket;
        char SHA[20];

        printf("Client is ready...\n");

        read(new_socket, &TotalPacket,sizeof(int));

        read(new_socket, SHA ,20);
        string BitBuffer = TotalFile[(string)SHA];

        send(new_socket, BitBuffer.c_str(), BitBuffer.size(), 0);

        int AvlPack = 0;
        for(int i = 0; i < TotalPacket; i++){
            if((BitBuffer[i]-'0')) AvlPack++;
        }
        send(new_socket, &AvlPack, sizeof(int), 0);
        read(new_socket, &ack, sizeof(int));
        
        close(new_socket);
        exit(0);
    }
    else{
        cout<<"[+]server busy...\n";
    }
}

void SendData(int new_socket){
    if(fork() == 0){
        char recvBuffer[BUFFER_SIZE];
        char sendBuffer[BUFFER_SIZE]; 
        memset(&recvBuffer, '\0',BUFFER_SIZE);
        memset(&sendBuffer, '\0',BUFFER_SIZE);

        int ack;

        int fileNameLength,LastPartSize = 0,TotalPacket;

        read(new_socket, &TotalPacket, sizeof(int));
        send(new_socket, &ack, sizeof(int), 0);
        read(new_socket, &LastPartSize, sizeof(int));
        send(new_socket, &ack, sizeof(int), 0);
        read(new_socket, &fileNameLength, sizeof(int));
        send(new_socket, &ack, sizeof(int), 0);
        read(new_socket, recvBuffer, fileNameLength-1);
        send(new_socket, &ack, sizeof(int), 0);

        ifstream input;
        input.open(recvBuffer,ifstream::binary); //Add Path here
        memset(&recvBuffer, '\0',BUFFER_SIZE);

        printf("> Sending data...\n");

        read(new_socket, &ack, sizeof(int));
        
        int index, PieceNo = -1; 
        while(PieceNo < TotalPacket-1){
            read(new_socket, &PieceNo, sizeof(int));
            index = PieceNo * DataSize;
            input.seekg (index);

            if(PieceNo == TotalPacket){ // Last packet issue may be there
                if(LastPartSize){
                    input.read(sendBuffer,LastPartSize);
                    send(new_socket , (char *)sendBuffer, LastPartSize,0);
                }
                else{
                    input.read(sendBuffer,DataSize);
                    send(new_socket , (char *)sendBuffer, DataSize,0);
                }
            }
            else{
                input.read(sendBuffer,DataSize);
                send(new_socket , (char *)sendBuffer, DataSize,0);
            }
            memset(&sendBuffer, '\0',BUFFER_SIZE);
        }
        printf("> Data sent!\n");
        close(new_socket);
        exit(0);
    }
    else{
        cout<<"[+]server busy...\n";
    }
}

void SetBitMap(int TotalPacket, int flag, string SHA){
    char *BitBuffer = new char[TotalPacket];
    if(flag){
        for(int i=0; i<TotalPacket; i++)
            BitBuffer[i] = '1';
    }
    else{
        for(int i=0; i<TotalPacket; i++)
            BitBuffer[i] = '0';
    }
    TotalFile[SHA] = (string)BitBuffer;
}