# chat-app-server using socket

This project has a server and a client application. The server manages and validates the user accounts based on username and password. The client can log into the server using TCP connection, get the list of other users on the server, send a text message to another user, check his/her own inbox messages, and chat with another user.


## Build 

Compile Server: 
```
gcc ConnectUser.c DieWithError.c handleClient.c main.c -o main
Run Server: ./main <port number>
```

Compile Client: 
```
gcc DieWithError.c ConnectToFriend.c  ChatWithFriendServer.c ChatWithFriendClient.c main.c listenToFriend.c  -o main
Run Client: ./main
```

##  Technologies
* C programming language 
* XCode 



