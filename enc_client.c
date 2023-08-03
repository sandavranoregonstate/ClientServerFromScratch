#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "some.h"

int main(int argc, char *argv[]) {

	// if the argument quantity is not correct, do print an error 
	if (argc < 4) {
		fprintf(stderr,"USAGE: %s plaintext key port\n", argv[0]);
		exit(0);
	}

	// use the abstract to client then request to encript 
	client_version_abstract(argv[1], argv[2], argv[3], "enc");

	return 0; 

}