#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

//buffer size
#define MAXSIZE 50

int main()
{
	int sockfd;	//socket descriptor, an integer (like a file handle)
	int retval;	//return value of connect() made to server
	int sendbytes;
	int recvbytes;
	int newsockfd;	//the file descriptor for the accepted client connection.
	
	socklen_t actuallen;	//to hold the size of the client address structure.
	struct sockaddr_in serveraddr;	//struct for server address
	struct sockaddr_in clientaddr;	//struct for client address
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
	serveraddr.sin_addr.s_addr=htons(INADDR_ANY);	//allowing the server to accept connections on any of the host's IP addresses.
	
	//binding
	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
    		printf("Binding error");
    		close(sockfd);
	}
	

	retval=listen(sockfd,1);	//The server is set to listen for incoming connections with a backlog of 1 (maximum pending connections).
	if(retval==-1)
	{
    		close(sockfd);
	}
	
	
	actuallen=sizeof(clientaddr);
	newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);	//waits for incoming connection creates new socket (newsockfd) for the client.
	
	if(newsockfd==-1)
	{
    		close(sockfd);
	}

	
	//server receives data from the client
	recvbytes=recv(newsockfd,buff,sizeof(buff),0);
	
	if(recvbytes==-1)
	{
    		close(sockfd);
    		close(newsockfd);
	}
	
	//recieved data printed
	puts(buff);
	printf("\n");
	
	
	//taking input
	printf("enter the text\n");
	scanf("%s",buff);
	
	//send data to server
	sendbytes=send(newsockfd,buff,sizeof(buff),0);
	
	if(sendbytes==-1)
	{
		printf("Sending failed");
    		close(sockfd);
    		close(newsockfd);
	}
	
	close(sockfd);
	close(newsockfd);
	
	return 0;
}
