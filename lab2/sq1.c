#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> // for inet_ntoa
#define MAXSIZE 90
int main()
{
    int sockfd, newsockfd;
    socklen_t actuallen;
    ssize_t recedbytes; // Use ssize_t for recv return value
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE]; // input buffer
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3389);
    serveraddr.sin_addr.s_addr = INADDR_ANY; // Correctly set the IP address
    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Binding error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, 1) == -1)
    {
        perror("Listen error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1)
    {
        perror("Accept error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
    if (recedbytes < 0)
    {
        perror("Recv Error");
    }
    else if (recedbytes == 0)
    {
        printf("Client disconnected\n");
    }
    else
    {
        buff[recedbytes] = '\0'; // Null-terminate the received string
        buff[strcspn(buff, "\n")] = 0;
        printf("Finding file *%s*\n", buff);
        FILE *fptr;
        fptr = fopen(buff, "r");
        if (fptr == NULL)
        {
            printf("File not found\n");
        }
        else
        {
            printf("File found\n");
            char ch;
            while ((ch = fgetc(fptr)) != EOF)
            {
                putchar(ch);
            }
            fclose(fptr); // Close the file pointer here
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
