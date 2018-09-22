#include<bits/stdc++.h>

using namespace std;

int TotalConn;
map<int,string> PacketNoAvl;
multimap<int,int> TotalAvlPack;

void PieceSelection(int TotalPacket,int TotalConn, int BitMap[]);
int main(){
    TotalConn = 3; int TotalPacket = 1794;

    TotalAvlPack.insert(make_pair(1794,0));
    TotalAvlPack.insert(make_pair(1794,1));
    TotalAvlPack.insert(make_pair(1794,2));

    char recvBuffer[1795];
    memset(&recvBuffer, '1', 1794);
    string BitString = (string)recvBuffer;
    
    PacketNoAvl[0] = BitString;
    PacketNoAvl[1] = BitString;

    // memset(&recvBuffer, '0', 1794);
    // recvBuffer[1] = '1';
    PacketNoAvl[2] = BitString;

    int BitMap[TotalPacket];
    memset(&BitMap, -1 ,sizeof(BitMap));

    PieceSelection(TotalPacket,TotalConn,BitMap);
    return 0;
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
    int i=0;
    for(;i<TotalPacket;i++){
        cout<<BitMap[i]; 
    }
    cout<<"\n"<<i<<" ";
}