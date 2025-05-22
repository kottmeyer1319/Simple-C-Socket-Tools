#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main()
{
    int clientFD;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    // create TCP socket for client
    clientFD = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFD < 0)
    {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) < 0)
    {
        perror("invalid address");
        close(clientFD);
        exit(EXIT_FAILURE);
    }

    // connect to the server
    if (connect(clientFD, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect error");
        close(clientFD);
        exit(EXIT_FAILURE);
    }

    // received data from server
    int bytes_recv = recv(clientFD, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_recv < 0)
    {
        perror("recv error");
        close(clientFD);
        exit(EXIT_FAILURE);
    }

    printf("Message from server: %s\n", buffer);

    close(clientFD);
    return 0;
}