# Inter Process Communication 

This project consists of five small programs that encrypt and decrypt information using a one-time pad-like system. The program is also be accessible from the command line using standard Unix features like input/output redirection, and job control. 

Details 

keygen.c : This file takes in a integer, which is the length of the key to get generated, then generates that key and returns it. 

enc_client.c : This file represents the client of the encryption application. The client of the encryption application take a plain text file, sends it to the server of the encryption application, then recieves an encrypted version of the plain text. These send and recieves has been implemented via web sockets and they are done via network inter process communication. 

enc_server.c : This file represents the server of the encryption application. The server of the ecryption application runs in the background, listening in a port that has been specified. It recieves a plain text file from just the client of the encryption application, encrypts it, then sends it to just the client of the encryption application. These send and recieves has been implemented via web sockets and they are done via network inter process communication. 



