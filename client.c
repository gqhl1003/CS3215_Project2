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

    char message[512];		    /* Your Name */

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    unsigned char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int i;			    /* Counter Value */

    char* serverIP = "127.0.0.1";

    memset(&sndBuf, 0, RCVBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Create a new TCP socket*/
    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

    if(clientSock < 0){
        printf(" creating client socket failed");
    }


    /* Construct the server address structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serverIP);
    serv_addr.sin_port = htons(4001);


    /* Establish connecction to the server */
    if(connect(clientSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0){
        printf("connecting server failed\n");
        return 1;
}
    printf("CONNECTED");

while(1){  
    /* Send the string to the server */
    printf("type here: ");
    scanf("%s", sndBuf);
    if(send(clientSock, sndBuf, strlen(sndBuf), 0) < 0){
        printf(" send() sent unexpected number of bytes");
        return 1;
    }

    /* Receive and print response from the server */
        if(recv(clientSock,rcvBuf,RCVBUFSIZE - 1, 0)<0){
            printf("recv falied"); 
            break;      
    }
        printf("Server reply: ");
        puts(rcvBuf);
}
    close(clientSock);
    return 0;
}

