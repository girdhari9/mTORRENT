#include "ClientServer.h"

vector<string> ConDetail;
vector<string> mTorrentFileData;

int FunctionCalling(int arg,string filename){
    if(arg < 2){
        cout<<"\n> Please give mtorrent file name or correctly!\n";
    }
    int TorrentFileD = open(filename.c_str(), O_RDONLY, O_SYNC);

    ReadFileByLine(TorrentFileD,1);
    // cout<<mTorrentFileData[0]<<" "<<mTorrentFileData[1]<<"\n";
    int sock = clientConnection(mTorrentFileData[0],atoi(mTorrentFileData[1].c_str()));
    if(sock == -1){
        cout<<"Error in connection\n";
        exit(0);
    }
    else{
        cout<<"> Connected with Tracker: "<<mTorrentFileData[0]<<":"<<mTorrentFileData[1]<<"";
    }
    ReadFileByLine(TorrentFileD,0);
    cout<<"> You are requesting to download: "<<mTorrentFileData[0];
    string FileName = mTorrentFileData[0];

    ReadFileByLine(TorrentFileD,0);
    cout<<"> File Size: "<<mTorrentFileData[0];
    int FileSize = stoi(mTorrentFileData[0]);

    ReadFileByLine(TorrentFileD,0);
    string SHA = ReadSHA(TorrentFileD);
    GetSeedersDetails(sock,SHA);

    cout<<"> Tracker Connection Closed!\n";
    cout<<ConDetail[0]<<" "<<ConDetail[1]<<"\n";
    sock = clientConnection(ConDetail[0],atoi(ConDetail[1].c_str()));
    
    if(sock == -1){
        cout<<"Error in connection\n";
        exit(0);
    }
    else{
        cout<<"> You are connected with: "<<ConDetail[0]<<":"<<ConDetail[1]<<"\n";
    }
    RecieveData(sock,FileName,FileSize);
    return 0;
}

int clientConnection(string IPaddress,int PORT){
    int sock = 0; 
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    
    // IPaddress = IPaddress.substr(2,9); //Ip address shoudl correct

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){ 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    return sock;
}

void GetSeedersDetails(int sock,string SHA){
    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE]; 
    memset(&recvBuffer, '\0',BUFFER_SIZE);
    memset(&sendBuffer, '\0',BUFFER_SIZE);

    cout<<"[+]system started...\n"; 
    cout<<"[+]system connecting...\n";

    cout<<"[+]system connected...\n";
    printf("> recieving data...\n");
    SHA = "abcdefghijklmnopqrst"; // Send data after reading from file
    int ack, DataLength;
    send(sock, SHA.c_str(), SHA_DIGEST_LENGTH, 0 );
    read(sock, &ack, sizeof(int));

    while(1){
        read(sock, &DataLength, sizeof(int));
        if(DataLength < 3){
            close(sock);
            return ;
        }
        send(sock, &ack, sizeof(int),0);
        read(sock, recvBuffer, DataLength);

        int index = 0;
        string ip_add, port_add, FileAddress;
        
        while(recvBuffer[index] != ' '){
            ip_add += recvBuffer[index];
            index++;
        }
        ConDetail.push_back(ip_add);

        while(recvBuffer[++index] != ' ')
            port_add += recvBuffer[index];
        ConDetail.push_back(port_add);

        while(recvBuffer[++index] != ' ')
            FileAddress += recvBuffer[index];
        ConDetail.push_back(FileAddress);

        //Connect using thread
    }
    return ;
}

void RecieveData(int sock,string recvFileName, int FileSize){
    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE]; 
    memset(&recvBuffer, '\0',BUFFER_SIZE);
    memset(&sendBuffer, '\0',BUFFER_SIZE);

    cout<<"[+]system started...\n"; 
    cout<<"[+]system connecting...\n";

    
    int read_size, ack;
    long long int count = 1,TotalSize = 0;

    int fileNameLength = recvFileName.length();
    send(sock , &fileNameLength , sizeof(int) , 0 );
    read(sock, &ack, sizeof(int));
    send(sock , recvFileName.c_str() , fileNameLength-1 , 0 );
    read(sock, &ack, sizeof(int));

    cout<<"[+]system connected...\n> recieving data...\n";

    recvFileName = "new_"+recvFileName.substr(0,recvFileName.size()-1);
    int recvFilePtr = open(recvFileName.c_str(), O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

    ack = 1;
    while((read_size = read(sock, recvBuffer, DataSize)) > 0){
        TotalSize += read_size;
        write(recvFilePtr,recvBuffer,read_size);
        memset(&recvBuffer, '\0',BUFFER_SIZE);
        send(sock, &count, sizeof(int), 0 );
        count++;
        if(FileSize == TotalSize){
            cout<<"> Data Received\n";
            break;
        }
    }
    close(sock);
    close(recvFilePtr);
    printf("> Connection closed!\n");
}

string ReadSHA(int TorrentFileD){
    char sendBuffer[BUFFER_SIZE];

    int read_size = read(TorrentFileD, sendBuffer, SHA_DIGEST_LENGTH);
    if(read_size == SHA_DIGEST_LENGTH){
        return sendBuffer;
    }
    return "-1";
}


void ReadFileByLine(int TrackerFileD,int flag){
    char sendBuffer[BUFFER_SIZE];
    int index = 0;

    mTorrentFileData.clear();
    memset(&sendBuffer, '\0',BUFFER_SIZE);
    while (read(TrackerFileD, &sendBuffer[index], 1) == 1) {
        if (sendBuffer[index] == '\n' || sendBuffer[index] == 0x0) {
            if(!flag){
                mTorrentFileData.push_back((string)sendBuffer);
                return ;
            }
            else{ 
                string sb = (string)sendBuffer;
                unsigned int found = sb.find(':');
                if (found != string::npos){
                    mTorrentFileData.push_back(sb.substr(0,found));
                    mTorrentFileData.push_back(sb.substr(found+1));
                }
                return ;
            }
        }
        index++;
    }
    return ;
}
