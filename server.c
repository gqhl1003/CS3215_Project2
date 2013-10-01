/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:	Qiang Guo
* PROJECT:	CS 3251 Project 1 - Professor Traynor
* DESCRIPTION:	Network Server Code
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>	  /* for printf() and fprintf() */
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>	  /* supports all sorts of functionality */
#include <unistd.h>	  /* for close() */
#include <string.h>	  /* support any string ops */
#include <openssl/evp.h>  /* for OpenSSL EVP digest libraries/SHA256 */

#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 40		/* Your name can be as many as 40 chars*/

/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;				/* Server Socket */
    int clientSock;				/* Client Socket */
    struct sockaddr_in changeServAddr;		/* Local address */
    struct sockaddr_in changeClntAddr;		/* Client address */
    unsigned short changeServPort = 4001;		/* Server port */
    unsigned int clntLen;			/* Length of address data struct */

    char nameBuf[BUFSIZE];			/* Buff to store name from client */
    unsigned char md_value[EVP_MAX_MD_SIZE];	/* Buff to store change result */
    EVP_MD_CTX *mdctx;				/* Digest data structure declaration */
    const EVP_MD *md;				/* Digest data structure declaration */
    int md_len, totleRecv;					/* Digest data structure size tracking */


    /* Create new TCP Socket for incoming requests*/
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

    if(serverSock < 0){
        perror(" creating socket failed");
    }

    /* Construct local address structure*/
    memset(&changeServAddr, 0, sizeof(changeServAddr));
    changeServAddr.sin_family = AF_INET;
    changeServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    changeServAddr.sin_port = htons(changeServPort);
    
    /* Bind to local address structure */
    if(bind(serverSock, (struct sockaddr *)&changeServAddr, sizeof(changeServAddr)) < 0){
        perror("bind() failed");
        /*exit(1);*/
    }

    //bind(serverSock, (struct sockaddr *)&changeServAddr, sizeof(changeServAddr));

    /* Listen for incoming connections */
    if(listen(serverSock, 10)< 0){
        perror("listen() failed");
        /*exit(1);*/
    }


    /* Loop server forever*/
    while(1)
    {
        
	/* Accept incoming connection */
     clntLen = sizeof(changeClntAddr);
     clientSock = accept(serverSock, (struct sockaddr *)&changeClntAddr, &clntLen);
        if(clientSock < 0)
        {
            printf(" accept failed");
        }
	/* Extract Your Name from the packet, store in nameBuf */
	 memset(nameBuf, 0, sizeof(nameBuf));  
        totleRecv = recv(clientSock, nameBuf, BUFSIZE, 0);
        if(totleRecv < 0){
            printf("receive failed");
        }
        else if(totleRecv == 0){
            printf(" finish");
        }
            printf(" message received %d\n ", totleRecv);
	/* Run this and return the final value in md_value to client */
	/* Takes the client name and changes it */
	/* Students should NOT touch this code */
	  OpenSSL_add_all_digests();
	  md = EVP_get_digestbyname("SHA256");
	  mdctx = EVP_MD_CTX_create();
	  EVP_DigestInit_ex(mdctx, md, NULL);
	  EVP_DigestUpdate(mdctx, nameBuf, strlen(nameBuf));
	  EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	  EVP_MD_CTX_destroy(mdctx);

	/* Return md_value to client */
      /*int i;
    printf("Transformed input is: ");
    for(i = 0; i < md_len; i++) printf("%02x", nameBuf[i]);
    printf("\n");*/
	send(clientSock,md_value,md_len,0);
    }

}

