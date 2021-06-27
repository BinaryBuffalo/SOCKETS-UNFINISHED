#include <stdio.h>
#include <string.h>
#include <stdlib.h> //provides exit(0);
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//
#include <netinet/udp.h> // udp headers
#include <netinet/ip.h> //ip headers
    struct pseudo_header{
        int var1;
        int var2;
        char name[25];
    };

int main(int argc, char* argv[]){

    //creating a UDP SOCKET 
     int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
     char datagram[4096] , source_ip[32], SPOOF[32] , *data , *pseudogram;
     memset (datagram, 0, 4096);
     struct iphdr *iph = (struct iphdr *) datagram;
     struct udphdr *udph = (struct udphdr *) (datagram + sizeof (struct ip));

    struct sockaddr_in sin;
	struct pseudo_header psh;
    //data 
    data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
	strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    strcpy(source_ip , "127.0.0.1");
    strcpy(SPOOF, "195.226.206.15");
    sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	sin.sin_addr.s_addr = in6_addr (source_ip);
    //
    iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data);
	iph->id = htonl (54321);	         //Id of this packet
	iph->frag_off = 0;                    
	iph->ttl = 255;
	iph->protocol = IPPROTO_UDP;
	iph->check = 0;		                //Set to 0 before calculating checksum
	iph->saddr = inet_addr (SPOOF); 	//Spoof the source ip address
	iph->daddr = sin.sin_addr.s_addr;
    //
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);

    udph->source = htons (6666);
	udph->dest = htons (8622);
	udph->len = htons(8 + strlen(data));	//tcp header size
	udph->check = 0;	//leave checksum 0 now, filled later by pseudo header

    psh.source_address = in6_addr( source_ip );
	psh.dest_address = sin.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );

    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
	pseudogram = malloc(psize);
	
	memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
	memcpy(pseudogram + sizeof(struct pseudo_header) , udph , sizeof(struct udphdr) + strlen(data));
	
	udph->check = csum( (unsigned short*) pseudogram , psize);
    int i;

while(i)
    {
		//Send the packet
		    if (sendto (s, datagram, iph->tot_len ,	0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
		        {
			            perror("sendto failed");
		        }
		//Data send successfully
		    else
		        {
		            	printf ("Packet Send. Length : %d \n" , iph->tot_len);
	    	    }
	}
	
	return 0;
}


