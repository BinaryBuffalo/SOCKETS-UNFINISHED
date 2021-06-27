#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char argv[]){
    int len, bytes_sent;
    char *msg = "Epstein didn't auth him self\n";
    struct sockaddr_in my_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM,0);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(5555);
    my_addr.sin_addr.s_addr = INADDR_ANY; //auto fill IP

    if(sockfd== -1){
        printf("[x] Socket Failed\n");
        return 1;
    }   
    else{
        printf("Socket Created\n");
    }
    len = strlen(msg);
    bytes_sent = send(sockfd, msg, len, 0);

    return 0;
}