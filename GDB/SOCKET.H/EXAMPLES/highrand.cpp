#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#define rape_syn 1111111111
#define gape_syn 9999999999
//               1804289383
        using namespace std;
int main(int argc, char* argv[]){

    int n = 0;
    n = rand();
    cout<<n<<endl;

    return 0;
}