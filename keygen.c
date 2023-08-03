#include <stdio.h> 
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "some.h" 

int main (int argc, char* argv[]) { 

	// declare variables 
	int i;
	int length = atoi(argv[1]); 

	char key[length+1];
	srand(time(0)); 

	// make this   
	for (i=0; i<length; i++) {
		int letter = rand() % 27;
		key[i] = variables[letter];
	}

	key[length] = '\0';
	printf("%s\n", key);

	return 0;
}