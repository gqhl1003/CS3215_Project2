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
#include <pthread.h>

#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 40		/* Your name can be as many as 40 chars*/

void *threadify(void *);
/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;				/* Server Socket */
    int clientSock;				/* Client Socket */
    int *new_sock;
    struct sockaddr_in changeServAddr;		/* Local address */
    struct sockaddr_in changeClntAddr;		/* Client address */
    int clntLen;
    /* Create new TCP Socket for incoming requests*/
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

    if(serverSock < 0){
        perror(" creating socket failed\n");
        return 1;
    }

    puts("socket created\n");

    /* Construct local address structure*/
    changeServAddr.sin_family = AF_INET;
    changeServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    changeServAddr.sin_port = htons(4001);
    
    /* Bind to local address structure */
    if(bind(serverSock, (struct sockaddr *)&changeServAddr, sizeof(changeServAddr)) < 0){
        perror("bind() failed\n");
        return 1;
    }
    printf("bind is done\n");

    /* Listen for incoming connections */
    if(listen(serverSock, 10)< 0){
        perror("listen() failed\n");
        return 1;
    }

    puts("listening ...... \n");
    clntLen = sizeof(changeClntAddr);


    /* Loop server forever*/
    while((clientSock = accept(serverSock, (struct sockaddr *)&changeClntAddr, &clntLen) ))
    {
     puts(" connection accepted \n");
     pthread_t tid;
     new_sock = malloc(1);
     *new_sock = clientSock;
     puts("creating a pthread now\n");
     if(pthread_create(&tid,NULL,threadify, (void*)new_sock)<0){
        puts("create thread failed");
        return 1;

     }
     printf("pthread has been created, and threadify assigned\n");
    }

    if(clientSock < 0){
        puts("accept connetion falied\n");
        return 1;
    }
    return 0;

   }

    void *threadify(void* arg){
        puts("threadify comes into the play....\n");
        char client_message[SNDBUFSIZE];
        int sock = *(int *)arg;
        int read_size;
        while((read_size = recv(sock, client_message, SNDBUFSIZE, 0))>0){
              // if(read(sock, &value, BUFSIZE) < BUFSIZE){
              //   close(sock);
              //   printf("threadify, value cant be read from  socket");
            //sending the message back to client
            write(sock, client_message, strlen(client_message));
            }

            if(read_size == 0){
                close(sock);
                return 0;
            }

            else if(read_size == -1){
                close(sock);
                puts("write falied\n");
                return 1;
            }
        free(sock);
        return 0;
    }


