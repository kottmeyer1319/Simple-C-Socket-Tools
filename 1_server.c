#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 12345

int main()
{
    int serverFD, clientFD;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // create TCP socket for server
    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0)
    {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // clear server address structure for safety
    memset(&server_addr, 0, sizeof(server_addr));
    
    // server address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to server address and port
    if (bind(serverFD, (struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
    {
        perror("bind error");
        close(serverFD);
        exit(EXIT_FAILURE);
    }

    // start listening
    if (listen(serverFD, 5) < 0)
    {
        perror("liste error");
        close(serverFD);
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d\n", SERVER_PORT);

    // accept a client connection
    clientFD = accept(serverFD, (struct sockaddr *)&client_addr, &client_len);
    if (clientFD < 0)
    {
        perror("accept error");
        close(serverFD);
        exit(EXIT_FAILURE);
    }

    // message send to client
    char *message = "Hello from server";
    if (send(clientFD, message, strlen(message), 0) < 0)
    {
        perror("send error");
        close(serverFD);
        exit(EXIT_FAILURE);
    }

    // close sockets
    close(clientFD);
    close(serverFD);
    return 0;
}