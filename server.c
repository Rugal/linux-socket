/*
 * File:   main.c
 * Author: Rugal Bernstein
 *
 * Created on March 20, 2016, 12:28 PM
 */

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "TCPSocket.h"
#define PORT 23333
#define CLIENT_INPUT_BUFFER_SIZE 1024

/*
 *
 */
int main(int argc, char** argv)
{
    printf("Server launched\n");
    int sd = createTCPSocket();
    struct sockaddr_in *serverAddress = bindTCPAddress(sd, PORT), clientAddress;
    //specify the quene length
    listen(sd, 3);
    socklen_t clientLength = sizeof (clientAddress);
    printf("Server listening\n");
    int client = accept(sd, (struct sockaddr *) &clientAddress, &clientLength);
    printf("Get connection from %u\n", clientAddress.sin_addr.s_addr);
    if(client < 0)
        exit(2);
    char input[CLIENT_INPUT_BUFFER_SIZE];
    while(read(client, input,CLIENT_INPUT_BUFFER_SIZE)>0 && strcmp(input, "exit")!=0)
        printf(">%s<\n", input);
    close(sd);
    close(client);
    free(serverAddress);
    return (EXIT_SUCCESS);
}
