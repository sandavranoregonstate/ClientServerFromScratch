
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "some.h"

void error(const char *msg) { 
	perror(msg); 
	exit(1); 
}

int server_version_abstract(char* port_arg, char* type_crypto)
{ 

	int wrongFile = 0; 
	int listenSocketFD ; 
	int establishedConnectionFD ; 
	int portNumber ; 
	int charsRead ; 

	int status;
	socklen_t sizeOfClientInfo;
	char buffer[512];
	char inputFileName[256];
	char keyFileName[256]; 

	char inputText[80000];
	char key[80000];
	char outputText[80000];
	struct sockaddr_in serverAddress, clientAddress;
	pid_t pid, sid;

	memset((char *)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = atoi(port_arg);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); 

	// check the given error 
	if (listenSocketFD < 0) { 
		error("ERROR opening socket");
	} 
	// check the given error 
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) { 
		error("ERROR on binding"); 
	}

	// listen to 5 different calls 
	listen(listenSocketFD, 5); 

	while(1) { 

		// connect to the given client 
		sizeOfClientInfo = sizeof(clientAddress);
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);
		
		// check the given error 
		if (establishedConnectionFD < 0) { 
			error("ERROR on accept"); 
		} 

		// fork therefore we could run multiple processes at the same time 
		pid = fork(); 
		switch (pid) {
			case -1: ; 

				error("ERROR creating fork");
				exit(0); 

				break;
			
			case 0: ; 

				// read the message provided here 
				memset(buffer, '\0', sizeof(buffer));
				charsRead = recv(establishedConnectionFD, buffer, sizeof(buffer)-1, 0); 

				// check the given error 
				if (charsRead < 0) { 
					error("ERROR reading from socket");
				} 
					
				// from the network message to the format ready for the crypto things 
				const char newline[2] = {'\n', '\0'};
				char *token = strtok(buffer, newline);
				strcpy(inputFileName, token);
				token = strtok(NULL, newline);
				strcpy(keyFileName, token);

				token = strtok(NULL, newline);

				// check the given error 
				if (strcmp(type_crypto, token)) {
					fprintf(stderr, "ERROR %s_client cannot use %s_server.\n", token, type_crypto );
					wrongFile = 1;
				}

				// if the file is valid 
				if (!wrongFile){

					FILE* inputFile = fopen(inputFileName, "r");
					fgets(inputText, 80000, inputFile);
					fclose(inputFile);
					inputText[strcspn(inputText, "\n")] = '\0';

					FILE* keyFile = fopen(keyFileName, "r");
					fgets(key, 80000, keyFile);
					fclose(keyFile);
					key[strcspn(key, "\n")] = '\0';
					
					if (!strcmp(type_crypto, "enc")) {
						encode(key, inputText, outputText, strlen(inputText));
					} 

					else if (!strcmp(type_crypto, "dec")) {
						decode(key, inputText, outputText, strlen(inputText));
					}
				}	 

				int uniquePid = getpid();
				
				char uniqueFile[32]; 
				
				sprintf(uniqueFile, "%s_f.%d", type_crypto, uniquePid);

				// do write the text possible  
				FILE* uniqueFD = fopen(uniqueFile, "w+"); 


				if (wrongFile) {
					fprintf(uniqueFD, "");
				} 
				else {
					fprintf(uniqueFD, "%s", outputText);
				} 

				// do close 
				fclose(uniqueFD);

				charsRead = send(establishedConnectionFD, uniqueFile, strlen(uniqueFile), 0); 
				
				// check the given error 
				if (charsRead < 0) { 
					error("ERROR writing to socket");
				} 
				
				// do close 
				close(establishedConnectionFD);
				establishedConnectionFD = -1;
				exit(0);

				break;

			default: ; 

		} 

				close(establishedConnectionFD);
				establishedConnectionFD = -1;
				wait(NULL);
	}

	close(listenSocketFD);
	return 0; 

} 
