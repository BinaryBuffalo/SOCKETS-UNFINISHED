#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
int recv_timeout(int s, int timeout);
int main(int argc, char argv[]){
    int len;
    char buffer[BUFSIZ];
    struct sockaddr_in my_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM,0);
    my_addr.sin_family = inet_addr("131.253.13.140");
    my_addr.sin_port = htons(80);
    my_addr.sin_addr.s_addr = INADDR_ANY; //auto fill IP
    if(sockfd== -1){
        printf("[x] Socket Failed\n");
        return 1;
    }   
    else{
        printf("Socket Created\n");
    }
    
    int connfd = connect(sockfd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr) );
    if(connfd == -1){
        fprintf(stderr, "[x] Connection Failed\n");
        return 1;
    }   else{
        printf("[+] Connected \n");
    }

    if((recv(sockfd, buffer , 4089, 0) ) < 0){
			usleep(5);
            return -1;
        }
    else{

    }
    return 0;
}