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
    if(sockfd <0) cout <<"Socket creation failed."<<endl;

    struct sockaddr_in serv,client;    
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    char buffer[256];
    socklen_t clientlen = sizeof(client);
    socklen_t serverlen = sizeof(serv);

    //socklen_t m = client;
    cout<<"Ready to send: Enter message:\n";
    fgets(buffer,256,stdin);

    sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&serv,serverlen);

    //recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&client,&clientlen);

    close(sockfd);
}
