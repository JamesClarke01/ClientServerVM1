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

	char username[20];
	char password[20];

	char userpass[40];


	if (argc < 2) // Test for correct number of arguments
	DieWithUserMessage("Parameter(s)", "<Server Address> <Server Port>");

	char *servIP = argv[1]; // First arg: server IP address (dotted quad)

	in_port_t servPort = atoi(argv[2]);  //second arg: server port 

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
	if(connection = connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("connect() failed");
	
	printf("Connected to Server!\n");
	
	for(;;){	
	 
	//send
	//prompt user to enter login credentials
	printf("Username: \n");
	scanf("%s",username);
	
	printf("Password: \n");
	scanf("%s", password);
	
	strcpy(userpass, username);
	
	strcat(userpass, " "); //space between parts
	strcat(userpass, password);

	//printf("%s\n", userpass); 


	snprintf(sendbuffer, sizeof(sendbuffer), "%s\r\n", userpass);  //store the contents of the input string in the sendBuffer
	
	printf("%s\n", sendbuffer);

	ssize_t numBytesSent = send(sock, sendbuffer, strlen(sendbuffer), 0);  //send sendBuffer content to the client

	if (numBytesSent < 0)
                DieWithSystemMessage("send() failed");
	

	//recieve
	while((numBytes = recv(sock, recvbuffer, BUFSIZE - 1, 0)) > 0)
	{
		recvbuffer[numBytes] = '\0'; // Terminate the string!
		fputs(recvbuffer, stdout); // Print the echo buffer
		/*Recive up to the buffer size
		(minus 1 to leave space for a null terminator) 
		bytes from the sender*/
		break;
	}	
	if (numBytes < 0)
		DieWithSystemMessage("recy() failed");
	else if (numBytes ==0)
		DieWithUserMessage("recy()", "connection closed prematurely");
	}//end loop
	fputc('\n', stdout); // Print a final linefeed

	close(sock);
	exit(0);
}
