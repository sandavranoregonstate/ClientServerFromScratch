#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "some.h"

int main(int argc, char *argv[]) {

	// if the argument quantity is not correct, do print an error 
	if (argc < 2) {
		fprintf(stderr,"USAGE: %s port\n", argv[0]);
		exit(1);
	}

	// use the abstract to server and then request to encript 
	server_version_abstract(argv[1], "enc");

	return 0;
}