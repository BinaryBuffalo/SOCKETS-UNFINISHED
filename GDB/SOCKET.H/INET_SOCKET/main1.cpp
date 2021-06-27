#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int make_socket (uint16_t port){
    int sock;
    struct sockaddr_in socketfd;

    //Creating the Socket
    sock= socket(PF_INET, SOCK_STREAM, 0);

    //how to check if the socket is working 
    if (sock < 0){
        perror ("socket");
        exit(EXIT_FAILURE);
    }

    //Give the socket a name
    name.sin_family = AF_INET;
    name.sin_port = htons (port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);
      

  if(bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0){
        perror("Failed\n");
        exit(EXIT_FAILURE);
    }
    return sock;

}