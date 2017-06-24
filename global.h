//
//  Glogal.h
//  Server_ChatApp
//
//  Created by Mayara Brandao Dusheyko on 2/27/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
//
#include <stdbool.h> /* boolean return */
#ifndef GLOBAL_H
#define GLOBAL_H


//users struct
struct login {
    char* username;
    char* password;
    bool connected;
};

//message struct
struct messageBox {
    char* username;
    char message[1000];
    //sender information
    char* IPaddress;
    char* portNumber;
};

extern struct login users[2];

extern struct messageBox inboxMessages[2];

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      USER CONNECTION AND VALIDATION FUNCTIONS       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HandleTCPClient(int clntSocket); /* TCP client handling function */

bool validateUser (char *username, char *password , struct login *usersList);

void ConnectUser(int clntSocket);

bool validateUser(char*, char* , struct login *); /*validade user function */

void DieWithError(char *errorMessage); /* Error handling function */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               MESSAGE FUNCTIONS                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void sendMessage (int clntSocket, char* aMessage); /* Send message to client */

void receiveMessage(int socket); /* Receive message from client */

char* getIP(); /* get Ip address from received message */

char* getPort(); /* get port number from received message */

void saveMessage(char* username); /* Saves message to user's mailbox */

int receiveMsgLength(int sock);

void sendConfirmation(int sock, char* fullMsgSize, int msgLen);

void receiveCofirmation(int sock, char* fullMsgSize, int msgLen );

int sendMsgLength(int sock, int msgLen);


#endif /* Header_h */
