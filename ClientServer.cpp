#include "ClientServer.h"
#include<thread>
using namespace std;

vector<string> v;
int PORT;

int main(int arg, char * args[]){
	char root_path[1000];
	getcwd(root_path,1000);
	string cmd; 
	while(1){
		cout<<"Enter Port: ";
		cin>>PORT;
		cout<<"Enter Command: ";
		cin>>cmd;

		if(cmd == "share"){
			string filename, mtorrentfilename;
			cin>>filename;
			cin>>mtorrentfilename;
			string SHA = mtorrentFile(filename,mtorrentfilename);
			thread client (FunctionCalling, cref(mtorrentfilename),1,SHA,root_path);
			client.join();
		}
		else if(cmd == "remove"){

		}
		else if(cmd == "get" || cmd == "Get"){
			string mtorrentfilename;
			cin>>mtorrentfilename;

			if(mtorrentfilename.size() > 9 && mtorrentfilename.substr(mtorrentfilename.size()-9) != ".mtorrent"){
        		cout<<"\n> Please give name of mtorrent file!\n";
		        return 0;
		    }
		    
			thread client (FunctionCalling, cref(mtorrentfilename),0,"No",root_path);
		    thread server (ServerConnection);

		    client.join();
		    server.join();
		}
		else{
			cout<<"Error: Command not found!\n";
		}

	    Jump:
		    cout<<"Do you want to continue? [Y/N]: ";
		    char c;
		    cin>>c;
		    if((c == 'N') || (c == 'n')){
		        cout<<"Connection Closed!\n";
		        break;
		    }
		    else if((c == 'Y') || (c == 'y'))
		    	continue;
		    else{
		    	cout<<"Plese enter corrent...\n";
			    goto Jump;
		    }
	}
    return 0; 
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