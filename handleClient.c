
//
//  HandleClient.c
//  Server_ChatApp
//
//  Created by Mayara Brandao Dusheyko on 2/25/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
//
#include "global.h"
#include <stdlib.h> /* for atoi() */
#include <stdbool.h> /* boolean return */
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h> /* for close() */
#include <string.h> /* for strcmp */
#include <ctype.h> /*tolower*/
#define RCVBUFSIZE 2000 /* Size of receive buffer */

char msgBuffer[RCVBUFSIZE];

void HandleTCPClient(int clntSocket) {
    int recvMsgSize; /* Size of received message */
    
    for (;;) { /* loop will run until client picks option 4 or 5 */
        
        /* clears buffer */
        bzero(msgBuffer,RCVBUFSIZE);
        
        /* Receive OPTION message from client */
        if ((recvMsgSize = recv(clntSocket, msgBuffer, RCVBUFSIZE, 0)) < 0) {
            DieWithError("recv() failed") ;
        }
        
        char option[3];
        strncpy(option, msgBuffer, 2);
        option[recvMsgSize] = '\0';
        
        int opt = atoi(option);
        
        switch (opt) {
            case 0:
                ConnectUser(clntSocket);
                break;
            case 1:
                printf("%s\n", "Return user list!");
                
                //copy users to userlist array
                char list[100];
                bzero(list, 99);
                sprintf(list, "List of users: \n%s \n%s", users[0].username,
                        users[1].username);
                
                //send confirmation msg to client
                int msgLen = strlen(list);
                list[msgLen] = '\0';
                
                //send user list
                if (send(clntSocket, list, msgLen, 0) != msgLen) {
                    DieWithError("send() failed");
                }
                printf("\nSent User List!\n");
                break;
                
            case 2:
                /* Receive USERNAME MESSAGE from client */
                receiveMessage(clntSocket);
                int size = strlen(msgBuffer);
                char name[100];
                memcpy(name, msgBuffer, size);
                name[size] = '\0';
                
                printf("\nA message to %s", name);
                
                //to lowercase
                int j = 0;
                while (name[j]) {
                    name[j] = tolower(name[j]);
                    j++;
                }
                
                /* Receive MESSAGE from client */
                receiveMessage(clntSocket);
                /* SAVE MESSAGE to mailbox */
                saveMessage(name);
                
                //Displays message
                printf("\nMessage: %s\n", msgBuffer);
                
                break;
                
            case 3:
                //Send Inbox messages back
                for (int i = 0; i < 2; i++) {
                    if (users[i].connected == true) {
                        //printf("\nSend back %s messages!\n", users[i].username);
                        int msgLen = strlen(inboxMessages[i].message);
                        if (msgLen > 0) {
                            inboxMessages[i].message[msgLen] = '\0';
                            sendMessage(clntSocket, inboxMessages[i].message);
                        } else {
                            printf("\nIbox is empty.\n");
                            sprintf(msgBuffer, "You have no Messages!");
                            int msgLen = strlen(msgBuffer);
                            msgBuffer[msgLen] = '\0';
                            sendMessage(clntSocket, msgBuffer);
                            printf("\nSend back %s messages!\n", users[i].username);
                        }
                    }
                }
                break;
                
            case 4:
                //Disconnect client
                printf("\nClient disconected\n");
                
                for (int i = 0; i < 2; i++) {
                    if (users[i].connected == true) {
                        users[i].connected = false;
                    }
                }
                close(clntSocket);
                return;
                
            case 5:
                //Disconnect client 2
                printf("\nClient 2 disconected\n");
                
                for (int i = 0; i < 2; i++) {
                    if (users[i].connected == true) {
                        users[i].connected = false;
                    }
                }
                close(clntSocket);
                return;
                
            default:
                break;
        }
    }
}


/* Receive confimation that message was received */
void receiveCofirmation(int sock, char* fullMsgSize, int msgLen ) {
    //reveive OK from server: Message received
    int bytesRcvd;
    if ((bytesRcvd = recv(sock, fullMsgSize, msgLen, 0)) <= 0) {
        DieWithError("recv() failed or connection closed prematurely");
    }
    fullMsgSize[bytesRcvd] = '\0'; /* Terminate the string! */
    
    //bzero(fullMsgSize, 5)
    
}

/* Send the length of the message to be send next */
int sendMsgLength(int sock, int msgLen) {
    
    //convert length to string
    char strMsgLen [5];
    sprintf(strMsgLen,"%d",msgLen);
    //getchar();
    
    //get length of message length string
    int sizeMsgLen = strlen(strMsgLen);
    
    /* Send the MESSAGE LENGTH to server */
    if (send(sock, strMsgLen, sizeMsgLen, 0) != sizeMsgLen) {
        DieWithError("send() sent a different number of bytes than expected");
    }
    
    receiveCofirmation(sock, strMsgLen, sizeMsgLen);
    
    return sizeMsgLen;
    
}

/* Send message */
void sendMessage (int sock,  char* aMessage) {
    //message size
    int msgLen = strlen(aMessage);
    
    //seng msg length
    sendMsgLength(sock, msgLen);
    
    
    /* Send the MESSAGE to server */
    if (send(sock, aMessage, msgLen, 0) != msgLen) {
        DieWithError("send() sent a different number of bytes than expected");
    }
    
    // bzero(friend_Server_msg_outbox, RCVBUFSIZE);
}

/* Send confirmation that message was received */
void sendConfirmation(int sock, char* fullMsgSize, int msgLen) {
    /* Send confirmation message to server */
    if (send(sock, fullMsgSize, msgLen, 0) != msgLen) {
        DieWithError("send() failed");
    }
}

/* Receive the length of the message to be received next */
int receiveMsgLength(int sock) {
    int bytesRcvd = 0;
    char fullMsgSize[25];
    bzero(fullMsgSize, 25);
    int size = 25;
    
    if ((bytesRcvd = recv(sock, fullMsgSize, 24, 0)) < 0) {
        DieWithError("recv() failed") ;
    }
    
    int msgLen = strlen(fullMsgSize);
    fullMsgSize[bytesRcvd] = '\0'; /* Terminate the string! */
    
    sendConfirmation(sock, fullMsgSize, msgLen);
    
    int fullMsg = atoi(fullMsgSize);
    
    return fullMsg;
    
}

/* Receive Message */
void receiveMessage (int sock) {
    //bzero(msgBuffer, RCVBUFSIZE);
    int bytesRcvd = 0;
    int totalBytesRcvd;
    
    /* Receive MESSAGE LENGTH from server */
    int fullMsg = receiveMsgLength(sock);
    
    /* Receive MESSAGE from server */
    totalBytesRcvd = 0;
    
    while (totalBytesRcvd < fullMsg) {
        if ((bytesRcvd = recv(sock, msgBuffer,  RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
        if (totalBytesRcvd == fullMsg) {
            msgBuffer[fullMsg] = '\0';
        }
    }
}

/* Get IP address from message*/
char* getIP() {
    char *ip = malloc(sizeof (char) * 20);
    int pos = 0;
    int leng = strlen(msgBuffer);
    for (int i = 0; i < leng; i++) {
        char c = msgBuffer[i];
        
        if (c == '\0') {
            break;
        }
        if (isdigit(c)) {
            ip[pos] = c;
            pos++;
            
        }
        if (c == '.') {
            ip[pos] = c;
            pos++;
        }
        if (c == ':') {
            break;
        }
    }
    
    ip[pos] = '\0';
    return ip;
}

/* Get port number */
char* getPort() {
    char *port = malloc(sizeof (char) * 20);
    int pos = 0;
    int leng = strlen(msgBuffer);
    
    //finds start position of port number
    int start = 0;
    char c = msgBuffer[start];
    while (start < leng && c!= ':') {
        start++;
        c = msgBuffer[start];
        
    }
    
    for (int i = start; i < leng; i++) {
        char c = msgBuffer[i];
        if (c == '\0') {
            break;
        }
        
        if (isdigit(c)) {
            port[pos] = c;
            pos++;
            
        }
    }
    port[pos] = '\0';
    return port;
}

/* Save message to user's mailbox */
void saveMessage(char *username) {
    
    //2: The number of mailboxes available
    for (int i = 0; i < 2; i++) {
        
        //Check if username is in mailboxList
        if (strcmp(inboxMessages[i].username, username) == 0) {
            //Parse message and save IP
            inboxMessages[i].IPaddress = getIP();
            
            //Save port number to mailbox
            inboxMessages[i].portNumber = getPort();
            
            //Save full message to user inbox
            strcpy(inboxMessages[i].message, msgBuffer);
        }
    }
}
