#include<bits/stdc++.h>
#include<thread>

#include<semaphore.h> 
#include<sys/wait.h>
using namespace std;

sem_t mutex1;

void write1(ofstream &output){
	ifstream input;
	input.open("input.txt",ifstream::binary);
	input.seekg(0,input.end);
	int len = input.tellg();
	int count = len/1024,last_part = 0;
	if(count % 2)
		last_part = len-count*1024;
	int index = 0; char c[1300];
	count/=2;
	while(count--){
		input.seekg (index);
		input.read(c,1024);
		
		sem_wait(&mutex1); 
		output.seekp(index);
		output.write(c,1024);
		sem_post(&mutex1);

		index+=2*1024;
	}
	if(last_part){
		input.seekg (index);
		input.read(c,last_part);
		
		sem_wait(&mutex1); 
		output.seekp(index);
		output.write(c,last_part);
		sem_post(&mutex1);
	}
}

void write2(ofstream &output){
	ifstream input;
	input.open("input.txt",ifstream::binary);
	input.seekg(0,input.end);
	int len = input.tellg();
	int count = len/1024,last_part = 0;
	if(count & 1)
		last_part = len-count*1024;
	int index = 1024; char c[1300];
	count/=2;
	while(count--){
		input.seekg (index);
		input.read(c,1024);
		
		sem_wait(&mutex1); 
		output.seekp(index);
		output.write(c,1024);
		sem_post(&mutex1);

		index+=2*1024;
	}
	if(last_part){
		input.seekg (index);
		input.read(c,last_part);
		
		sem_wait(&mutex1); 
		output.seekp(index);
		output.write(c,last_part);
		sem_post(&mutex1);
	}
}

int main(){
	sem_init(&mutex1, 0, 1); 
	ofstream output1;
	output1.open("a.txt", ofstream::binary);
	thread f1 (write1,ref(output1));
	thread f2 (write2,ref(output1));
	f1.join();
	f2.join();
	sem_destroy(&mutex1); 
}

