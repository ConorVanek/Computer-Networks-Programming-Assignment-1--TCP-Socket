#include <arpa/inet.h> 
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> 
#include <sys/socket.h>
#include <unistd.h> 
#include <ctype.h>
#define MAX 256
#define SA struct sockaddr

void prompt(int sockfd)
{
    char buff[MAX];
    int n;
    while(1) {
        bzero(buff, sizeof(buff));
        printf("Input: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
         printf("Output:\n%s", buff);
         if ((strncmp(buff, "quit", 4)) == 0) {
             printf("Client Exiting...\n");
             break;
         }
    }
}
 
int main(int argc, char *argv[])
{
    int sockfd, port;
    struct sockaddr_in servaddr;
 
	port = atoi(argv[1]);
    // Create and verify socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP address and port
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(port);
 
    // connect to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
 
    // Begin user prompt
    prompt(sockfd);
 
    // close the socket
    close(sockfd);
	return 0;
}