#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "some.h"

void error(const char *msg, int value) { 

	perror(msg); 
	exit(value); 

} 

int client_version_abstract(char* plaintext, char* key, char* port_arg, char* type_crypto)
{	

	// do the following things initial   
	int i, j;
	int socketFD ; 
	int portNumber ; 
	int charsWritten ; 
	int charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[512]; 

    // from given char to int 
	int port = atoi(port_arg);

	FILE* textFile = fopen(plaintext, "r");
	char text_plaintext[80000];
	fgets(text_plaintext, 80000, textFile);
	fclose(textFile);
	text_plaintext[strcspn(text_plaintext, "\n")] = '\0';

	FILE* keyFile = fopen(key, "r");
	char text_key[80000];
	fgets(text_key, 80000, keyFile);
	fclose(keyFile);
	text_key[strcspn(text_key, "\n")] = '\0'; 

	// make sure the string of length is not the different from each other 
	int textLength = strlen(text_plaintext);
	int keyLength = strlen(text_key);
	if (keyLength < textLength) {
		fprintf(stderr, "ERROR The key is shorter than the plaintext.\n");
		exit(1);
	}
	
	// make sure there is only the valid characters 

	for (i=0; i<keyLength; i++) {
		for (j=0; j<28; j++) {
			if (j == 27) {
				fprintf(stderr, "Key contains invalid characters.\n");
			}
			if (text_key[i] == variables[j]) {
				break;
			}
		}
	} 

	for (i=0; i<textLength; i++) {
		for (j=0; j<28; j++) {
			if (j == 27) {
				fprintf(stderr, "ERROR Plaintext contains invalid characters.\n");
				exit(1);
			}
			if (text_plaintext[i] == variables[j]) {
				break;
			}
		}
	}

	// set up for the network 
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = port;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber); 

	serverHostInfo = gethostbyname("localhost"); 

	// check the given error 
	if (serverHostInfo == NULL) {
		fprintf(stderr, "CLIENT: ERROR, no such host\n");
		exit(0);
	} 

	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);
	socketFD = socket(AF_INET, SOCK_STREAM, 0); 
	
	// check the given error 
	if (socketFD < 0) { 
		error("CLIENT: ERROR opening socket", 1);
	} 

	// check the given error 
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) { 
		error("CLIENT: ERROR connecting", 1);
	}
		
	// clear out the value there  
	memset(buffer, '\0', sizeof(buffer)); 

	sprintf(buffer, "%s\n%s\n%s", plaintext, key, type_crypto); 
	charsWritten = send(socketFD, buffer, strlen(buffer), 0); 

	// check the given error 
	if (charsWritten < 0) { 
		error("CLIENT: ERROR writing to socket", 1);
	} 

	// check the given error 
	if (charsWritten < strlen(buffer)) { 
		printf("CLIENT: WARNIGN: Not all data written to socket!\n");
	} 

	// clear out the value there 
	memset(buffer, '\0', sizeof(buffer)); 

	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); 

	if (charsRead < 0) { 
		error("CLIENT: ERROR reading from socket", 1);
	} 

	FILE* recvFile = fopen(buffer, "r");
	char output[80000];
	fgets(output, 80000, recvFile);
	fclose(recvFile);

	remove(buffer);
	printf("%s\n", output);
	close(socketFD);
	return 0; 

}