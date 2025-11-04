#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    // check correct arguments
    if (argc != 4)
    {
        printf("Usage: %s <IP_ADDRESS> <START_PORT> <END_PORT>\n", argv[0]);
        return 1;
    }

    // IP address and ports from arguments
    char *ip_address = argv[1];
    int start_port = atoi(argv[2]);
    int end_port = atoi(argv[3]);

    printf("Scanning %s from port %d to %d\n", ip_address, start_port, end_port);

    // scan loop
    for (int port = start_port; port <= end_port; port++)
    {
        int sockFD = socket(AF_INET, SOCK_STREAM, 0);
        if (sockFD < 0)
        {
            perror("socket error");
            continue;
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip_address, &addr.sin_addr) < 0)
        {
            perror("invalid address");
            close(sockFD);
            exit(EXIT_FAILURE);
        }

        // connect to target address
        int connect_status = connect(sockFD, (struct sockaddr *)&addr, sizeof(addr));
        if (connect_status == 0)
        {
            printf("Port %d is OPENED\n", port);
        }
        else
        {
            printf("Port %d is CLOSED\n", port);
        }

        close(sockFD);
    }

    printf("Scan complete\n");

    return 0;
}
