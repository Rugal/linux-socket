#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "TCPSocket.h"

int createTCPSocket()
{
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        //        perror("Unable to create socket\n");
        exit(1);
    }
    return sd;
}

struct sockaddr_in* bindTCPAddress(int sd, unsigned int port)
{
    //Trying to bind socket with specific address and port
    struct sockaddr_in* serverAddress = malloc(sizeof (struct sockaddr_in));
    {
        memset(serverAddress, 0, sizeof (struct sockaddr_in));
        serverAddress->sin_family = AF_INET;
        serverAddress->sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddress->sin_port = htons(port);
    }
    if (bind(sd, (struct sockaddr *) serverAddress, sizeof (struct sockaddr_in)) < 0)
    {
        //        fprintf(stderr, "Unable to bind socket on 0.0.0.0:%u \n", port);
        exit(1);
    }
    return serverAddress;
}
