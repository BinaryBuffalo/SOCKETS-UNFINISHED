#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]){
    
    int sockfd = socket(AF_INET,SOCK_STREAM ,0 );
    struct sockaddr_in my_addr;
    
    if(sockfd == -1){
        fprintf(stderr, "[x] Socket Failed\n");
        return 1;
    }   else{
        printf("[+] Created Socket\n");
    }
    my_addr.sin_family = AF_INET; //host byte order
    my_addr.sin_port = htons(5555);
    my_addr.sin_addr.s_addr = INADDR_ANY; //auto my IP
    memset(&(my_addr.sin_zero), '\0', 8); 
    //set the rest of the Header to 0
    int bindfd = bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
    if(bindfd == -1){
        fprintf(stderr, "[x] Binding Failed\n");
        return 1;
    }   else{
        printf("[+] Binding Complete\n");
    }
    int connfd = connect(sockfd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr) );
    if(connfd == -1){
        fprintf(stderr, "[x] Connection Failed\n");
        return 1;
    }   else{
        printf("[+] Connected \n");
    }
    int listfd = listen(sockfd, 5);
    if(listfd == -1){
        printf("[x] Failed Listening \n");
    }   else{
        printf("[+] Listening \n");    }










   

    return 0;
}

