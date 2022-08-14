#include "server.h"
#include <signal.h>
#define PORT 3322

int sockfd;                  // socket file descriptor
int acceptClientReq;         // to accept a connection request from a client
void signal_handler(int sig) // signal handler for SIGINT & SIGTSTP
{
    // close socket file descriptor and exit
    printf("\n");
    printf("Server is shutting down...\n");
    close(acceptClientReq);
    close(sockfd);
    printf("Server is down.\n");
    exit(0);
}

int main()
{
    int clientLength, status;
    int protocol = 0;
    struct sockaddr_in serverAddress, clientAddress;
    sockfd = socket(AF_INET, SOCK_STREAM, protocol); // (int domain, int type, int protocol);
    if (sockfd < 0)
    {
        printf("Error while creating socket\n");
        exit(0);
    }
    else
    {
        printf("Socket is generated successfully\n");
    }
    memset(&serverAddress, '\0', sizeof(serverAddress)); // zero structure out
    serverAddress.sin_family = AF_INET;                  // match the socket() call
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);   // bind to any local address
    serverAddress.sin_port = htons(PORT);                // specify port to listen on
    if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        int port = ntohs(serverAddress.sin_port);
        printf("Error binding on port %d \n", port);
        exit(0);
    }
    else
    {
        printf("Socket is bound\n");
        printf("Waiting for connection from client...\n");
    }
    listen(sockfd, 5); // (int sockfd, int backlog) -  listen for incoming connections
    while (1)
    {
        if (signal(SIGINT, signal_handler) == SIG_ERR) // Ctrl + c signal
        {
            printf("\ncan't catch SIGINT\n");
            exit(1);
        }

        if (signal(SIGTSTP, signal_handler) == SIG_ERR) // Ctrl + z signal
        {
            printf("\ncan't catch SIGTSTP\n");
            exit(1);
        }
        clientLength = sizeof(clientAddress);
        acceptClientReq = accept(sockfd, (struct sockaddr *)&clientAddress, &clientLength); // to accept a connection request from a client
        if (acceptClientReq < 0)
        {
            printf("\nError accepting request from client\n");
            exit(0);
        }
        else
        {
            printf("Client connection request accepted!\n");
        }
        pid_t pid = fork();
        if (pid == 0) // child process
        {
            close(sockfd);
            mainloop(acceptClientReq);
            close(acceptClientReq);
            exit(0);
        }
        else
        {
            wait(&status); // wait for child process to finish
        }
        close(acceptClientReq);
    }
    return 0;
}