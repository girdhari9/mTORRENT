#include "mtorrent.h"
#include <openssl/sha.h>

#define FILE_SIZE 524288
#define HASH_LENGHT 20
using namespace std;

void mtorrentFile(){
	string SrcFileName = "input.txt";
	string TorrentFileName = "first1.mtorrent";

	struct stat fileStat; 
	int err = stat(SrcFileName.c_str(), &fileStat); 
	if (0 != err){
		cout<<"Error occured in file";
		return ;
	} 
	string l = to_string(fileStat.st_size);
    int mtorrentPtr = open(TorrentFileName.c_str(), O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

	string Tracker1 = "127.0.0.1:9000\n";
    string Tracker2 = "127.0.0.1:9001\n";
    write(mtorrentPtr,Tracker1.c_str(),strlen(Tracker1.c_str()));
    write(mtorrentPtr,Tracker2.c_str(),strlen(Tracker2.c_str()));
    
    write(mtorrentPtr,SrcFileName.c_str(),strlen(SrcFileName.c_str()));
	write(mtorrentPtr,"\n",1);
    write(mtorrentPtr,l.c_str(),strlen(l.c_str()));
	write(mtorrentPtr,"\n",1);

	CalculateSHA(SrcFileName,mtorrentPtr);
}


int CalculateSHA(string SrcFileName,int mtorrentPtr){
    unsigned char src_buffer[FILE_SIZE];
    unsigned char hashChunk[SHA_DIGEST_LENGTH];
    unsigned char fullhash[SHA_DIGEST_LENGTH];

    SHA_CTX sc;

    int srcFilePtr = open(SrcFileName.c_str(), O_RDONLY);

    SHA1_Init(&sc);
    SHA1_Update(&sc, SrcFileName.c_str(), SrcFileName.length());
    for (;;) {
        size_t read_size;
        bzero(src_buffer, FILE_SIZE);
        bzero(hashChunk, SHA_DIGEST_LENGTH);
        read_size = read(srcFilePtr,src_buffer,FILE_SIZE);
        // cout<<read_size<<"\n";
        if (read_size == 0)
            break;
        SHA1((unsigned char*) src_buffer, read_size, hashChunk);
        write(mtorrentPtr,hashChunk,SHA_DIGEST_LENGTH);
        // cout<<src_buffer;
        SHA1_Update(&sc, src_buffer, read_size);
    }

    close(srcFilePtr);

    SHA1_Final(fullhash, &sc);
    write(mtorrentPtr,"\n",1);
    write(mtorrentPtr,fullhash,SHA_DIGEST_LENGTH);
    close(mtorrentPtr);
    return 0;
}