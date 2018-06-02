//
//  chatclient.cpp
//  Test
//
//  Created by Allan Ngei on 2/10/18.
//  Copyright © 2018 Allan Ngei. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

using namespace std;

int initiateContact(const char* servHostName, const char* servPortNo){
	int sock;
	int connection = -1;
//	sock = socket(AF_INET,	SOCK_STREAM, 0);
//	
//	if(sock == -1){
//		cerr << "Socket set-up failed." << endl;
//		return sock;
//	}
	
//	struct sockaddr_in serverAddr;
//	
//	serverAddr.sin_family = AF_INET;
//	
//	serverAddr.sin_port = htons(atoi(servPortNo));
//	
//	serverAddr.sin_addr.s_addr = inet_addr(servHostName);
//	
//	socklen_t len = sizeof(serverAddr);
//	
//	connection = connect(sock, (const struct sockaddr*) &serverAddr, len);

	struct addrinfo *result, *iterator;
//	const char service[] = "http";
	int a = getaddrinfo(servHostName, servPortNo, NULL, &result);
	result->ai_socktype = 1;
	if(a != 0){
		cerr << "Connection could not be extablished." << endl;
		return -1;
	}
	
	for (iterator = result; iterator != NULL; iterator = iterator->ai_next) {
        	sock = socket(iterator->ai_family, iterator->ai_socktype, iterator->ai_protocol);
        	if (sock == -1) continue;

		connection = connect(sock, iterator->ai_addr, iterator->ai_addrlen);
       		if (connection != -1) break;                  

       		close(sock);
    	}
    	
	if(connection == -1){
		cerr << "Connection could not be extablished." << endl;
		return -1;
	}
	
	return sock;
}

bool sendMessage(int sock, const char* message){
	ssize_t sent;
	
	size_t msgLength = strlen(message);
	msgLength += 2;
	
	sent = send(sock, message, msgLength, 0);
	
	if(sent == -1){
		cerr << "Message failed to send." << endl;
		return false;
	}
	
	return true;
}

ssize_t recvMessage(int sock, const char* message){
	int maxMsgLen = 500;
	
	ssize_t recd = recv(sock, (char*) message, maxMsgLen, 0);
	
	if(recd < 0){
		cerr << "Error on incoming messgage." << endl;
	}
	
	return recd;
}

int main(int args, char* argv[]){
//	if(!initiateContact(argv[1], argv[2])){
//		exit(1);
//	}
	char handle[10];
	cout << "What nickname (10 characters max) would you like to identify you? ";
	cin >> handle;
	
	string handle2(handle);
	handle2 = handle2 + "> ";
	const char* handle3 = handle2.c_str();
	size_t extra = strlen(handle3);
	
	char hostname[20];
	gethostname(hostname, 20);
//	struct hostent* localhost  = gethostbyname(hostname);
//	string hostname = "24.216.213.246";
	string portno = "50001";
	
//	const char* hostname2 = hostname.c_str();
	const char* portno2 = portno.c_str();
	
	int socket = initiateContact(hostname, portno2);
		
	if(socket == -1){
		exit(1);
	}
	
	const char* quit = "/quit";
	
	while(true){
		char buffer[501];
		char buffer2[501];
		char buffer3[501 + extra];
		memset(buffer, '\0', 501);
		memset(buffer2, '\n', 501);
		memset(buffer3, '\n', 501 + extra);
		strcpy(buffer3, handle3);
//		while(recvMessage(socket, buffer) <= 0){
//			continue;
//		}
	
		recvMessage(socket, (const char*) buffer);
	
		if(strcmp(buffer, quit) == 0){
			if(!sendMessage(socket, buffer)){
				exit(2);
			}
			if(close(socket) == 0){
				cout << "Chat session has terminated!" << endl;
				break;
			}
			else{
				cerr << "Socket could not be closed." << endl;
				exit(3);
			}
		}
		
		cout << buffer << endl << handle2;
		
		cin >> buffer2;
		strcat(buffer3, buffer2);
//		cin.getline(buffer2, 500);
		
		if(!sendMessage(socket, buffer3)){
			exit(2);
		}
		
//		free(buffer);
	}
	return 0;
}
