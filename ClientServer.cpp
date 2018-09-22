#include "ClientServer.h"
#include<thread>
using namespace std;

vector<string> v;

int main(int arg, char * args[]){
	string cmd; 
	while(1){
		cout<<"Enter Command: ";
		cin>>cmd;
		tokenized(cmd);

		if(v[0] == "share"){
			mtorrentFile(v[1],v[2]);

		}
		else if(v[0] == "remove"){

		}
		else if(v[0] == "get" || v[0] == "Get"){
			thread client (FunctionCalling, cref(filename));
		    thread server (ServerConnection);

		    client.join();
		    server.join();
		}
		

	    Jump:
		    cout<<"Do you want to continue? [Y/N]: ";
		    char c;
		    cin>>c;
		    if((c = 'N') || (c == 'n')){
		    	terminate();
		        cout<<"Connection Closed!\n";
		        break;
		    }
		    else if((c = 'Y') || (c == 'y'))
		    	continue;
		    else{
		    	cout<<"Plese enter corrent...\n";
			    goto Jump;
		    }
	}
    return 0; 
}

void tokenized(string s){
    v.erase(v.begin(),v.end());
    for(unsigned int i = 0;i < s.size(); i++){
        vector<char> sub_v;
        vector<char>::iterator it;
        while(s[i] != ' ' && i < s.size()){
            if(s[i] == 92){ 
                sub_v.push_back(32);
                i += 2;
            }
            else{
                sub_v.push_back(s[i]);
                i++;
            }
        }
        string t;
        for(unsigned int j = 0; j < sub_v.size();j++)
            t += sub_v[j];
        v.push_back(t);
    }
}