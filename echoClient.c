#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include "Practical.h"

int main(int argc, char *argv[]) {
	char recvbuffer[BUFSIZE]; // I/O buffer
	char sendbuffer[BUFSIZE]; 
	int numBytes = 0;

	if (argc < 3) // Test for correct number of arguments
	DieWithUserMessage("Parameter(s)", "<Server Address> <Server Port>");

	char *servIP = argv[1]; // First arg: server IP address (dotted quad)

	in_port_t servPort = atoi(argv[2]);  //second arg: server port 

	char *echoString = argv[3];

	// Create a reliable, stream socket using TCP
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Construct the server address structure
	struct sockaddr_in servAddr; // Server address
	memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
	servAddr.sin_family = AF_INET; // IPv4 address family

	// Convert address
	int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
	if(rtnVal == 0)
		DieWithUserMessage("inet_pton() failed", "invalid address string");
	else if (rtnVal < 0)
		DieWithSystemMessage("inet_pton() failed");
	servAddr.sin_port = htons(servPort); // Server port

	// Establish the connection to the echo server
	if(connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("connect() failed");
	
	//send
	snprintf(sendbuffer, sizeof(sendbuffer), "%s\r\n", echoString);  //store the contents of the input string in the sendBuffer

	ssize_t numBytesSent = send(sock, sendbuffer, strlen(sendbuffer), 0);  //send sendBuffer content to the client

	if (numBytesSent < 0)
                DieWithSystemMessage("send() failed");
	

	close(sock);
	exit(0);
}
