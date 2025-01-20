#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#define MAXSIZE 50

int main()
{
    int sockfd, retval;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr;
    int choice, num, res;
    char buff[MAXSIZE]; // input buffer
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("\nSocket Creation Error");
    }
    printf("%i", sockfd);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3389);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Connection error");
    }
    while (1)
    {
        printf("enter the file name\n");
        fgets(buff, sizeof(buff), stdin);
        sentbytes = send(sockfd, buff, sizeof(buff), 0);
        if (sentbytes == -1)
        {
            perror("Send error");
            close(sockfd);
            exit(1);
        }

        // recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        // if (recedbytes == -1)
        // {
        //     perror("Recv error");
        //     close(sockfd);
        //     exit(1);
        // }
        // printf("%s", buff);
    }
    close(sockfd);
    return 0;
}
