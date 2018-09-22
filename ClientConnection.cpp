#include "ClientServer.h"

vector<string> ConDetail;
vector<string> mTorrentFileData;
sem_t mutex1;
int TotalConn;
map<int,string> PacketNoAvl;
multimap<int,int> TotalAvlPack;


int FunctionCalling(string filename){
    sem_init(&mutex1, 0, 1);

    if(filename.size() > 9 && filename.substr(filename.size()-9) != ".mtorrent"){
        cout<<"\n> Please give name of mtorrent file!\n";
        return 0;
    }
    int TorrentFileD = open(filename.c_str(), O_RDONLY, O_SYNC);

    ReadFileByLine(TorrentFileD,1);

    int sock = TrackerConnection(mTorrentFileData[0],atoi(mTorrentFileData[1].c_str()));
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

    // Connections using multithreads
    
    int ConDetailIndex = 0;
    TotalConn = ConDetail.size()/3;

    int LastPartSize = 0;
    int TotalPacket = FileSize/DataSize;
    LastPartSize = FileSize - TotalPacket * DataSize;

    if(LastPartSize) TotalPacket++;

    int BitMap[TotalPacket];
    memset(&BitMap, -1, sizeof(BitMap));

    thread *threadptr = new thread[TotalConn];
    for(int thread_n = 0; thread_n < TotalConn; thread_n++,ConDetailIndex+=3){
        threadptr[thread_n] = thread(BitVectorRequestToServers,ConDetail[ConDetailIndex],atoi(ConDetail[ConDetailIndex+1].c_str()),ref(FileName),thread_n);
    }
    
    for(int thread_n=0; thread_n < TotalConn; thread_n++,ConDetailIndex++){
        threadptr[thread_n].join();
    }

    delete [] threadptr;

    PieceSelection(TotalPacket,TotalConn,BitMap);

    vector<int> BitMap1(BitMap, BitMap + sizeof(BitMap) / sizeof(BitMap[0]));

    ConDetailIndex = 0;
    ofstream output;
    string NewFileName = "new_" + FileName.substr(0,FileName.size()-1);
    output.open(NewFileName.c_str(), ofstream::binary);

    thread *Threadptr = new thread[TotalConn];
    for(int thread_n = 0; thread_n < TotalConn; thread_n++,ConDetailIndex+=3){
        Threadptr[thread_n] = thread(clientConnection,ConDetail[ConDetailIndex],atoi(ConDetail[ConDetailIndex+1].c_str()),ref(FileName),FileSize,LastPartSize,TotalPacket,BitMap1,thread_n,ref(output));
    }
    
    for(int thread_n=0; thread_n < TotalConn; thread_n++,ConDetailIndex++){
        Threadptr[thread_n].join();
    }
    //End of MultithNewFileNameread
    output.close();
    
    // RecieveData(sock,FileName,FileSize);
    sem_destroy(&mutex1);
    return 0;
}

int TrackerConnection(string IPaddress,int PORT){
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
    
    if(inet_pton(AF_INET, IPaddress.c_str() , &serv_addr.sin_addr)<=0){ 
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

    int ConDetailIndex = 0;
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
        memset(&recvBuffer, '\0',BUFFER_SIZE);
        if(ConDetailIndex == 0)
            ConDetail[ConDetailIndex] = ConDetail[ConDetailIndex].substr(2,9);
        ConDetailIndex+=3;
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
int clientConnection(string IPaddress,int PORT,string &recvFileName, int FileSize, int LastPartSize, int TotalPacket, vector<int> BitMap1, int thread_n, ofstream &output){
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
    
    if(inet_pton(AF_INET, IPaddress.c_str() , &serv_addr.sin_addr)<=0){ 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    if(sock == -1){
        cout<<"Error in connection\n";
        exit(0);
    }
    else{
        sem_wait(&mutex1);
        cout<<"[+]system started...\n"; 
        cout<<"[+]system connecting...\n";
        cout<<"> You are connected with: "<<IPaddress<<":"<<PORT<<"\n";
        sem_post(&mutex1);
    }
    int DataRequest = 2;
    send(sock , &DataRequest , sizeof(int) , 0 );
    RecieveBitVector(sock,recvFileName,FileSize,LastPartSize,TotalPacket,BitMap1,thread_n, ref(output));
    return 0;
}

void RecieveBitVector(int sock,string recvFileName, int FileSize, int LastPartSize, int TotalPacket, vector<int> BitMap1, int thread_n, ofstream &output){
    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE]; 
    memset(&recvBuffer, '\0',BUFFER_SIZE);
    memset(&sendBuffer, '\0',BUFFER_SIZE);

    int ack;

    int fileNameLength = recvFileName.length();

    send(sock , &TotalPacket , sizeof(int) , 0 );
    read(sock, &ack, sizeof(int));
    send(sock , &LastPartSize , sizeof(int) , 0 );
    read(sock, &ack, sizeof(int));
    send(sock , &fileNameLength , sizeof(int) , 0 );
    read(sock, &ack, sizeof(int));
    send(sock , recvFileName.c_str() , fileNameLength-1 , 0 );
    read(sock, &ack, sizeof(int));

    cout<<"[+]system connected...\n> recieving data...\n";
    fflush(stdout);
    ack = 1;

    int PieceNo=0, index;
    send(sock , &ack, sizeof(int) , 0);

    for(int PieceNo = 0; PieceNo < TotalPacket;){
        if(BitMap1[PieceNo] == thread_n){
            send(sock, &PieceNo, sizeof(int), 0);
            read(sock, recvBuffer, DataSize);

            sem_wait(&mutex1);
                index = PieceNo * DataSize;
                output.seekp(index);
                output.write(recvBuffer,DataSize);
                PieceNo++;
            sem_post(&mutex1);
            memset(&recvBuffer, '\0',BUFFER_SIZE);
        }
        else{
            sem_wait(&mutex1);
                PieceNo++;
            sem_post(&mutex1);
        }
    }
    send(sock, &PieceNo, sizeof(int), 0);
    if(BitMap1[PieceNo] == thread_n){
        if(LastPartSize){
            read(sock, recvBuffer, LastPartSize);
            sem_wait(&mutex1);
                index = PieceNo * DataSize;
                output.seekp(index);
                output.write(recvBuffer,LastPartSize);
                index += TotalConn * DataSize;
                PieceNo++;
            sem_post(&mutex1);
        }
        else{
            read(sock, recvBuffer, DataSize);
            sem_wait(&mutex1);
                index = PieceNo * DataSize;
                output.seekp(index);
                output.write(recvBuffer,DataSize);
                index += TotalConn * DataSize;
                PieceNo++;
            sem_post(&mutex1);
        }
        memset(&recvBuffer, '\0',BUFFER_SIZE);
    }
    cout<<"> Data Received\n";
    close(sock);
    printf("> Connection closed!\n");
    return ;
}

int Connection(string IPaddress,int PORT){
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
    
    if(inet_pton(AF_INET, IPaddress.c_str() , &serv_addr.sin_addr)<=0){ 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    else{
        sem_wait(&mutex1);
            cout<<"> You are connected with: "<<IPaddress<<":"<<PORT<<"\n";
        sem_post(&mutex1);
    }
    return sock;
}

void BitVectorRequestToServers(string IPaddress, int PORT, string FileName, int thread_n){
    int sock = Connection(IPaddress,PORT);

    int BitMapRequest = 1;
    send(sock , &BitMapRequest , sizeof(int) , 0 );

    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE];
    memset(&recvBuffer, '\0',BUFFER_SIZE);
    memset(&sendBuffer, '\0',BUFFER_SIZE);
    int ack = 1, AvlPack;

    int fileNameLength = FileName.length();

    send(sock , &fileNameLength , sizeof(int) , 0 );
    send(sock , FileName.c_str() , fileNameLength-1 , 0 );

    read(sock, recvBuffer, BUFFER_SIZE);
    read(sock, &AvlPack, sizeof(int));
    send(sock , &ack , sizeof(int) , 0 );

    string BitString = (string)recvBuffer;
    sem_wait(&mutex1);
        TotalAvlPack.insert(make_pair(AvlPack,thread_n));
        PacketNoAvl[thread_n] = BitString;
    sem_post(&mutex1);
    return ;
}

void PieceSelection(int TotalPacket,int TotalConn, int BitMap[]){
    int MinPackFromOneServer = (TotalPacket/TotalConn)+1;
    int PackIndex[TotalConn], TotalPackAssign = 0;

    memset(&PackIndex, 0, sizeof(PackIndex));
    for(auto it = TotalAvlPack.begin(); it != TotalAvlPack.end(); it++){
        string PackS = PacketNoAvl[it->second];
        int TotalSelectedPiece = 0, Loop = 0;

        while((TotalSelectedPiece < MinPackFromOneServer) && (it->first > TotalSelectedPiece) && (Loop < TotalPacket)){
            if(BitMap[Loop] == -1 && (PackS[Loop]-'0')){
                BitMap[Loop] = it->second;
                TotalSelectedPiece++;
            }
            Loop++;
        }
        if(Loop < TotalPacket && it->first >= MinPackFromOneServer)
            PackIndex[it->second] = Loop;
    }
    for(int i=0;i<TotalPacket;i++)
        if(BitMap[i] >= 0)
            TotalPackAssign++;
        
    int index;
    while(1) {
        if(TotalPackAssign < TotalPacket){
            for(int ConNo=0; ConNo < TotalConn; ConNo++){
                if((index = PackIndex[ConNo]) > 0){
                    string PackS = PacketNoAvl[ConNo];
                    while(!(PackS[index]-'0') && (index < TotalPacket)){
                        index++;
                    } 
                    if((index < TotalPacket) && (BitMap[index] == -1)){
                        BitMap[index] = ConNo;
                        TotalPackAssign++;
                        PackIndex[ConNo] = ++index;
                    }
                }
            }
        }
        else break;
    }
    return ;
}