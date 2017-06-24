//
//  ConnectUser.c
//  Server_ChatApp
//
//  Created by Mayara Brandao Dusheyko on 2/26/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
//

#include "global.h"
#include <stdio.h>
#include <stdlib.h> /* for atoi() */
#include <stdbool.h> /* boolean return */
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h> /* for close() */
#include <string.h> /* for strcmp */
#include <ctype.h> /*tolower*/
#define RCVBUFSIZE 1024 /* Size of receive buffer */

void ConnectUser(int clntSocket) {
    
    char msgBuffer[RCVBUFSIZE]; /* Buffer for receive string msg */
    int recvMsgSize = 0; /* Size of received message */
    
    /* Receive USER NAME message from client */
    if ((recvMsgSize = (int)recv(clntSocket, msgBuffer, RCVBUFSIZE, 0)) < 0) {
        DieWithError("recv() failed") ;
    }
    
    msgBuffer[recvMsgSize] = '\0';
    char user[10];
    memcpy(user, &msgBuffer, recvMsgSize);
    user[recvMsgSize] = '\0';
    
    bzero(msgBuffer,RCVBUFSIZE);
    
    /* Receive PASSWORD message from client */
    if ((recvMsgSize = (int) recv(clntSocket, msgBuffer, RCVBUFSIZE, 0)) < 0) {
        DieWithError("recv() failed") ;
    }
    
    msgBuffer[recvMsgSize] = '\0';
    char pass[5];
    memcpy(pass, &msgBuffer, recvMsgSize);
    pass[recvMsgSize] = '\0';
    
    char *response;
    int msgLen = 0;
    
    //Check username and password
    bool valid = false;
    if ((valid = validateUser(user, pass, users) == true)) {
        printf("%s\n", "Valid user");
        response = "Login was successful";
        msgLen = strlen(response);
        
    } else {
        printf("%s\n", "Invalid user");
        response = "Login was not successful";
        msgLen = strlen(response);
    }
    
    //Send confirmation msg to client
    if (send(clntSocket, response, msgLen, 0) != msgLen) {
        DieWithError("send() failed");
    }
    
}

/* Function to validate users */
bool validateUser (char *username, char *password , struct login *usersList) {
    //to lowercase
    int j = 0;
    while (username[j]) {
        username[j] = tolower(username[j]);
        j++;
    }
    
    //checks username and password
    for (int i = 0; i < 2; i++) {
        char* name = usersList[i].username;
        char* pass = usersList[i].password;
        
        if (strcmp(username, name) == 0) {
            if (strcmp(password, pass) == 0) {
                printf("Log in user name is: %s\n", username);
                printf("Log in password is: %s\n", password);
                users[i].connected = true;
                return true;
            }
        }
    }
    
    return false;
}
