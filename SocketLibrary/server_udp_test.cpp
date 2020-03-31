#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

uint16_t PORT = 53000;

void error( char *msg)
{
 perror(msg);
 exit(EXIT_FAILURE);
}

int main()
{
    int sockfd;
    sockfd = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    // Meaning, AF_INET (IPv4 protocol) refers to addresses from the internet, IP addresses specifically.
    // PF_INET refers to anything in the protocol, usually sockets/ports.
    // SOCK_STREAM: TCP(reliable, connection oriented); SOCK_DGRAM: UDP(unreliable, connectionless);
    // IPPROTO_UDP - specifying the network protocol
    
    if(sockfd <0) cout <<"Socket creation failed."<<endl;

    struct sockaddr_in serv,client;
    socklen_t serverlen = sizeof(serv);
 
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd,(struct sockaddr *)&serv,serverlen) != 0){
      cout<<"Bind Failed."<<endl;  
    }

    char buffer[256];
    socklen_t clientlen = sizeof(client);
    //socklen_t m = client;
    cout<<"Wainting for a message"<<endl;
    int rc= recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client,&clientlen);
    if(rc<0) cout<<"ERROR READING FROM SOCKET"<< endl;

    cout<<"Message received: "<<buffer;

   // int rp= sendto(sockfd,"hi",2,0,(struct sockaddr *)&client,clientlen);
   // if(rp<0) cout<<"ERROR writing to SOCKET"<<endl;
}