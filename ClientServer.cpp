#include "ClientServer.h"
#include<thread>
using namespace std;

vector<string> v;
int PORT;
string MYIP, MYPORT, TRACKER1IP, TRACKER1PORT, DOWNLOADLOG;
map<string,string> TotalFile;
vector<string> TotalSharedFile;
char root_path[1000];
vector<string> Download;

vector<string> LogVector;
vector<string> CreateLog;
vector<string> UpdateLog;

int main(int arg, char * args[]){
	getcwd(root_path,1000);
	string cmd, ClientLog = "clientlog.txt";

	MYIP = args[1];
	MYPORT = args[2];
	TRACKER1IP = args[3];
	TRACKER1PORT = args[4];
	DOWNLOADLOG = args[5];
	PORT = stoi(MYPORT);

	ReadLogFile(ClientLog);
	UpdateToTracker();

	while(1){

		cout<<"Enter Command: ";
		cin>>cmd;

		if(cmd == "share"){
			string filename, mtorrentfilename;
			cin>>filename;
			cin>>mtorrentfilename;
			TotalSharedFile.push_back(mtorrentfilename);
			string SHA = mtorrentFile(filename,mtorrentfilename,TRACKER1IP,TRACKER1PORT);
			UpdateDownloadLog(1,filename);

			CreateLog.push_back(mtorrentfilename);
			CreateLog.push_back(SHA);
			CreateLog.push_back(root_path);
			UpdateLogFile(ClientLog);
			ClientLog.erase(ClientLog.begin(),ClientLog.end());
			
			thread client (FunctionCalling, cref(mtorrentfilename),1,SHA,root_path);
			thread server (ServerConnection);
			client.join();
			server.detach();
		}
		else if(cmd == "remove"){
			string mtorrentfilename;
			cin>>mtorrentfilename;
			if(mtorrentfilename.size() > 9 && mtorrentfilename.substr(mtorrentfilename.size()-9) != ".mtorrent"){
        		cout<<"\n> Please give correct name of mtorrent file!\n";
		        return 0;
		    }

		    thread client (FunctionCalling, cref(mtorrentfilename),2,"No",root_path);
		    client.join();
		}
		else if(cmd == "get" || cmd == "Get"){
			string mtorrentfilename;
			cin>>mtorrentfilename;

			if(mtorrentfilename.size() > 9 && mtorrentfilename.substr(mtorrentfilename.size()-9) != ".mtorrent"){
        		cout<<"\n> Please give correct name of mtorrent file!\n";
		        return 0;
		    }

			thread client (FunctionCalling, cref(mtorrentfilename),0,"No",root_path);
			UpdateDownloadLog(0,mtorrentfilename);

			thread server (ServerConnection);
		    client.join();
		    server.detach();
		}
		else if(cmd == "show" || cmd == "Show"){
			string dw; cin>>dw;
			ShowDownloadDetail();
		}
		else{
			cout<<"Error: Command not found!\n";
		}

	    Jump:
		    cout<<"Do you want to continue? [Y/N]: ";
		    char c;
		    cin>>c;
		    if((c == 'N') || (c == 'n')){
		    	UpdateLogFile(ClientLog);
		    	for(unsigned int i = 0; i < TotalSharedFile.size(); i++){
		    		thread client (FunctionCalling, cref(TotalSharedFile[i]),2,"No",root_path);
		    		client.join();
		    	}
		        cout<<"Connection Closed!\n";
		        break;
		    }
		    else if((c == 'Y') || (c == 'y'))
		    	continue;
		    else{
		    	cout<<"Plese enter correct...\n";
			    goto Jump;
		    }
	}
    return 0; 
}


void UpdateDownloadLog(int flag,string filename){
	ifstream TrackerFile(DOWNLOADLOG.c_str(), ifstream::binary);
	string detail;
    if (TrackerFile.is_open()){
    	while(getline (TrackerFile,detail)){
			Download.push_back(detail);
		}
    	if(flag){
    		string detail ="[S] " + MYIP + ":" + MYPORT + " " + root_path + "/" + filename + "\n"; 
    		Download.push_back(detail);
        }
        else{
        	string detail ="[D] " + MYIP + ":" + MYPORT + " " + root_path + "/" + filename + "\n"; 
    		Download.push_back(detail);
        }
        TrackerFile.close();

        ofstream TrackerFileD(DOWNLOADLOG.c_str(), ofstream::binary);
        for(unsigned int i = 0; i < Download.size(); i++){
        	TrackerFileD << Download[i];
        	TrackerFileD << "\n";
        }
        TrackerFileD.close();
    }
}

void ShowDownloadDetail(){
	ifstream TrackerFile(DOWNLOADLOG.c_str(), ifstream::binary);
    if (TrackerFile.is_open()){
    	string detail;
    	while(getline (TrackerFile,detail)){
    		cout<<detail<<"\n";
    	}
        TrackerFile.close();
    }
}

void UpdateLogFile(string ClientLog){
	ifstream TrackerFile(ClientLog, ifstream::binary);
	string detail;
    if (TrackerFile.is_open()){
    	while( getline(TrackerFile,detail) ){
    		stringstream Log(detail); // Used for breaking words 
            string mtorrentfilename,SHA,FilePath; // to store individual words 
        	Log >> mtorrentfilename;
            UpdateLog.push_back(mtorrentfilename);
	        Log >> SHA;
	        UpdateLog.push_back(SHA);
	        Log >> FilePath;
	        UpdateLog.push_back(FilePath);
    	}
    	TrackerFile.close();

    	ofstream TrackerFileD(ClientLog, ofstream::binary);
        for(unsigned int i = 0; i < UpdateLog.size(); i += 3){
        	TrackerFileD << UpdateLog[i];
        	TrackerFileD << " ";
        	TrackerFileD << UpdateLog[i+1];
        	TrackerFileD << " ";
        	TrackerFileD << UpdateLog[i+2];
        	TrackerFileD << " ";
        	TrackerFileD << "\n";
        }
        for(unsigned int i = 0; i < CreateLog.size(); i += 3){
        	TrackerFileD << CreateLog[i];
        	TrackerFileD << " ";
        	TrackerFileD << CreateLog[i+1];
        	TrackerFileD << " ";
        	TrackerFileD << CreateLog[i+2];
        	TrackerFileD << " ";
        	TrackerFileD << "\n";
        }
        TrackerFileD.close();
    }
}

void ReadLogFile(string ClientLog){
	ifstream TrackerFile(ClientLog.c_str(), ifstream::binary);
    if (TrackerFile.is_open()){
    	string detail;
    	while(getline (TrackerFile,detail)){
    		stringstream Log(detail); // Used for breaking words 
            string mtorrentfilename,SHA,FilePath; // to store individual words 
            	Log >> mtorrentfilename;
                LogVector.push_back(mtorrentfilename);
		        Log >> SHA;
		        LogVector.push_back(SHA);
		        Log >> FilePath;
		        LogVector.push_back(FilePath);
    	}
        TrackerFile.close();
    }
}

void UpdateToTracker(){
	for(unsigned int i = 0; i < LogVector.size(); i += 3){
		string mtorrentfilename, SHA, root_path;
		mtorrentfilename = LogVector.front(); LogVector.pop_back();
		SHA = LogVector.front(); LogVector.pop_back();
		root_path = LogVector.front(); LogVector.pop_back();
		thread client (FunctionCalling, cref(mtorrentfilename),1,SHA,root_path);
		client.join();
	}
}

// void tokenized(string s){
// 	cout<<s<<" !";
//     v.erase(v.begin(),v.end());
//     for(unsigned int i = 0;i < s.size(); i++){
//         vector<char> sub_v;
//         while(s[i] != ' ' && i < s.size()){
//             if(s[i] == 92){ 
//                 sub_v.push_back(32);
//                 i += 2;
//             }
//             else{
//                 sub_v.push_back(s[i]);
//                 i++;
//             }
//         }
//         string t;
//         for(unsigned int j = 0; j < sub_v.size();j++)
//             t += sub_v[j];
//         cout<<t<<" ";
//         v.push_back(t);
//     }
// }