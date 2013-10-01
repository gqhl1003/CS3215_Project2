/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Qiang Guo
* PROJECT:	CS 3251 Project 1 - Professor Traynor
* DESCRIPTION:	Network Client Code
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/evp.h>	    /* for OpenSSL EVP digest libraries/SHA256 */

/* Constants */
#define RCVBUFSIZE 512		    /* The receive buffer size */
#define SNDBUFSIZE 512		    /* The send buffer size */
#define MDLEN 32

/* The main function */
int main(int argc, char *argv[])
{

    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* The server address */

    char *studentName;		    /* Your Name */

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    unsigned char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int i;			    /* Counter Value */

    char* serverIP = "130.207.114.21";

    /* Get the Student Name from the command line */
    if (argc != 2) 
    {
	printf("Incorrect input format. The correct format is:\n\tnameChanger your_name\n");
	exit(1);
    }
    studentName = argv[1];
    memset(&sndBuf, 0, RCVBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Create a new TCP socket*/
    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

    if(clientSock < 0){
        printf(" creating client socket failed");
    }


    /* Construct the server address structure */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serverIP);
    serv_addr.sin_port = htons(4001);


    /* Establish connecction to the server */
    if(connect(clientSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0)
        printf("connecting server failed");
    
    /* Send the string to the server */
    i = strlen(studentName);
    printf("my string length is %d bytes \n",i );
    if(send(clientSock, studentName, i, 0)!= i){
        printf(" send() sent unexpected number of bytes");
    }


    /* Receive and print response from the server */
    unsigned int totalRecv = 0;
    while(totalRecv < 32){
        i = recv(clientSock,rcvBuf,RCVBUFSIZE - 1, 0);
         totalRecv += i;
        if(totalRecv < 0)
            printf("revv() failed");
        else if (totalRecv == 0)
            printf("connection closed");
            
    }
        /*i = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);

*/
    printf("i received %s \n", rcvBuf);
    printf("total received %d bytes\n", i);


    printf("%s\n", studentName);
    printf("Transformed input is: ");
    for(i = 0; i < MDLEN; i++) printf("%02x", rcvBuf[i]);
    printf("\n");

    return 0;
}

