#include <stdio.h>
#include <string.h>
#include "some.h" 

void string_to_integer(char input[], int output[], int size) {
	int i, j;
	for (i=0; i<size; i++) {
		for (j=0; j<27; j++) {
			if (input[i] == variables[j]) {
				output[i] = j;
				break;
			}
		}
	}
}

void integer_to_string(int input[], char output[], int size) {
	int i;
	for (i=0; i<size; i++) {
		output[i] = variables[input[i]];
	}
}

void encode(char key[], char input[], char output[], int size) { 

	int i; 
	
	// input from string to integer 
	int int_in[size];
	string_to_integer(input, int_in, size);


	// not input from string to integer 
	int int_key[size];
	string_to_integer(key, int_key, size); 


	// the math 
	int int_out[size];
	for (i=0; i<size; i++) {
		int_out[i] = (int_in[i] + int_key[i]) % 27;
	}

	// method from integer to string 
	integer_to_string(int_out, output, size);
	output[size] = '\0'; 

}

void decode(char key[], char input[], char output[], int size) { 

	int i; 

	// input from string to integer 
	int int_in[size]; 
	string_to_integer(input, int_in, size);


	// not input from string to integer 
	int int_key[size];
	string_to_integer(key, int_key, size);


	// the math 
	int int_out[size];
	for (i=0; i<size; i++) {
		int_out[i] = int_in[i] - int_key[i];
		if (int_out[i] < 0) int_out[i] += 27;
	}

	// method from integer to string 
	integer_to_string(int_out, output, size);
	output[size] = '\0'; 
	
} 
