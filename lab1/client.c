#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

//buffer size
#define MAXSIZE 50

int main()
{
	int sockfd;	//socket descriptor, an integer (like a file handle)
	int retval;	//return value of connect() made to server
	int sendbytes;
	int recvbytes;
	struct sockaddr_in serveraddr;	//Declaration of a sockaddr_in structure which is used to specify an endpoint address for IPv4.
	char buff[MAXSIZE];	//input buffer
	
	sockfd=socket(AF_INET,SOCK_STREAM,0);	//sock_stream for tcp and sock_dgm for udp
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
	}
	printf("%i\n",sockfd);
	
	//initializes the serveraddr structure:
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");	//server addr
	
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	
	//taking input
	printf("enter the text\n");
	scanf("%s",buff);
	
	//send data to server
	sendbytes=send(sockfd,buff,sizeof(buff),0);
	
	if(sendbytes==-1)
	{
		printf("Sending failed");
		close(sockfd);
	}
	
	//recieves data from server
	recvbytes=recv(sockfd,buff,sizeof(buff),0);
	
	//print recieved msg on server
	puts(buff);
	printf("\n");
	
	close(sockfd);
	
	return 0;
}
