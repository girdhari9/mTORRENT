#include "ClientServer.h"
#include<thread>
using namespace std;

int main(int arg, char * args[]){ 
	string filename = (string)args[1]; 	
	thread client (FunctionCalling, arg, cref(filename));
    thread server (ServerConnection);

    client.join();
    server.join();
    return 0; 
} 