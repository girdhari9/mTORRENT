#include "ClientServer.h"
#include<thread>
using namespace std;

int main(int arg, char * args[]){
	string filename; 
	while(1){
		
		cout<<"Enter Filename: ";
		cin>>filename; 	
		thread client (FunctionCalling, cref(filename));
	    thread server (ServerConnection);

	    client.join();
	    server.join();

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