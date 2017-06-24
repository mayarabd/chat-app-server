//  main.c
//  Server_ChatApp
//
//  Created by Mayara Brandao Dusheyko on 2/25/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
//

#include "global.h"
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define MAXPENDING 5 /* Maximum outstanding connection requests */

//initialize users information
struct login users[2] = { [0] = {.username = "alice", .password = "12345"},
    [1] = {.username = "bob", .password = "00000"}};

//users inbox
struct messageBox inboxMessages[2] = { [0] = {.username = "alice"}, [1] = {.username = "bob"}};

int main(int argc, char *argv[])
{
    int servSock; /* Socket descriptor for server */
    int clntSock; /* Socket descriptor for client */
    struct sockaddr_in servAddr; /* Local address */
    struct sockaddr_in clientAddr; /* Client address */
    unsigned short servPort; /* Server port */
    unsigned int clntLen; /* Length of client address data structure */
    
    if (argc != 2) /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]) ;
        exit(1);
    }
    
    // servPort = 8000;
    servPort = atoi(argv[1]); /* First arg: local port */
    
    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError( "socket () failed") ;
    
    /* Construct local address structure */
    memset(&servAddr, 0, sizeof(servAddr)); /* Zero out structure */
    servAddr.sin_family = AF_INET; /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(servPort); /* Local port */
    
    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        DieWithError ( "bind () failed");
    
    printf("Server Started!\n");
    printf("Listen on 127.0.0.1s : %hu \n", servPort);
    
    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed") ;
    
    
    for (;;) /* Run forever */
    {
        int connClients = 0;
        /* Set the size of the in-out parameter */
        clntLen = sizeof(clientAddr);
        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &clientAddr, &clntLen)) < 0)
            DieWithError("accept() failed");
        
        /*number of clients connected */
        connClients++;
        /* clntSock is connected to a client! */
        printf("\nClient %i connected\n", connClients);
        
        HandleTCPClient (clntSock);
        // printf("\nSERVER - waiting to connect another client");
    }
    /* NOT REACHED */
}
