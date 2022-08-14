#include "client.h"
#define PORT 3322

int socketFd;
void signal_handler(int sig) // signal handler for SIGINT & SIGTSTP
{
    // close socket file descriptor and exit
    printf("\n");
    printf("Client is shutting down...\n");
    close(socketFd);
    printf("Client is down.\n");
    printf("\n");
    exit(0);
}

int main()
{
    struct sockaddr_in serverAddress;                  // server address
    int protocol = 0;                                  // protocol type
    socketFd = socket(AF_INET, SOCK_STREAM, protocol); // create socket
    bzero(&serverAddress, sizeof(serverAddress));
    if (socketFd < 0) // check if socket is created
    {
        printf("Client: Error while creating socket\n");
        exit(0);
    }
    else
    {
        printf("Client: Socket is generated successfully\n");
    }
    serverAddress.sin_family = AF_INET;                                                  // set address family
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);                                   // set IP address
    serverAddress.sin_port = htons(PORT);                                                // set port number
    if (connect(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // connect to server
    {
        int port = ntohs(serverAddress.sin_port);
        printf("Can not connect to server on PORT %d\n", port); // error on connecting to server
        exit(0);
    }
    else
    {
        int port = ntohs(serverAddress.sin_port);
        printf("Connected to server successfully on port %d\n", port); // print connection status
    }
    char bffer[9999] = "";
    char fileN[33];
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
        bzero(bffer, sizeof(bffer));                         // clear buffer
        printf("\n$FTP-Client:");                            // prompt for input
        scanf("%[^\n]%*c", bffer);                           // read input from user
        write(socketFd, bffer, strlen(bffer));               // send command to server
        printf("\nMessage has sent to server: %s\n", bffer); // print message which is sent to server
        if (strlen(bffer) > 7)
        {
            int bfferLength = strlen(bffer) - 5;
            strncpy(fileN, bffer + 5, bfferLength);
            fileN[bfferLength] = '\0'; // Null terminate the string
        }
        read(socketFd, bffer, sizeof(bffer)); // read from server
        if (strcmp(bffer, "\n502 Connection Termination\n") == 0)
        {
            printf("\n502 Connection Termination\n");
            break;
        }
        if (strcmp(bffer, "225 Data connection is currently open but there is no transfer in progress.\n") == 0)
        {
            printf("%s", bffer);
            printf("\n250 File sending is in process\n");
            sleep(3);
            FILE *filePtr;               // file pointer
            filePtr = fopen(fileN, "r"); // open file for reading
            if (filePtr == NULL)
            {
                printf("Error: Can not open the file: %s \n", fileN);
            }
            else
            {
                char c;
                bzero(bffer, sizeof(bffer)); // clear buffer
                int iterate = 0;
                while (1)
                {
                    while (((c = fgetc(filePtr)) != EOF) && (strlen(bffer) - 1 != sizeof(bffer)))
                    {
                        bffer[iterate] = c; // read character from file and store in buffer
                        iterate++;          // increment counter
                    }
                    bffer[iterate] = '\0'; // Null terminate the string
                    if (c == EOF)
                    {
                        write(socketFd, bffer, strlen(bffer)); // send command to server
                        break;                                 // break if end of file is reached
                    }
                    if (strlen(bffer) == sizeof(bffer))
                    {
                        write(socketFd, bffer, strlen(bffer)); // send command to server
                        bzero(bffer, sizeof(bffer));           // clear buffer
                        iterate = 0;                           // reset iterate
                    }
                }
                printf("File %s has been sent successfully! \n", fileN); // print message which is received from server
                fclose(filePtr);
            }
        }
        else if (strcmp(bffer, "226 Data connection opened and no transfer in progress.\n") == 0)
        {
            printf("%s", bffer);         // print message which is received from server
            FILE *filePtr;               // file pointer
            filePtr = fopen(fileN, "w"); // open file for writing
            if (filePtr == NULL)
            {
                printf("Error: Can not open given file: %s \n", fileN);
            }
            else
            {
                bzero(bffer, sizeof(bffer)); // clear buffer
                while (1)
                {
                    read(socketFd, bffer, sizeof(bffer)); // read from server
                    fputs(bffer, filePtr);                // write to file
                    if (strlen(bffer) != sizeof(bffer))   // break if end of file is reached
                    {
                        break;
                    }
                    bzero(bffer, sizeof(bffer)); // clear buffer
                }
                printf("File %s has been received successfully! \n", fileN); // print message which is received from server
                fclose(filePtr);
            }
        }
        else
        {
            printf("%s", bffer); // print message which is received from server
        }
    }
    close(socketFd); // close socket
    return 0;
}