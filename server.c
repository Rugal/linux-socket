/*
 * File:   main.c
 * Author: Rugal Bernstein
 *
 * Created on March 20, 2016, 12:28 PM
 */
#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include "TCPSocket.h"
#define PORT 23333
#define CLIENT_INPUT_BUFFER_SIZE 1024

void handler(int s)
{
    int status;
    wait(&status);
    printf("One child terminated\n");
}


void childProcess(int socket, struct sockaddr_in * client)
{
    char input[CLIENT_INPUT_BUFFER_SIZE];
    while(read(socket, input,CLIENT_INPUT_BUFFER_SIZE)>0 && strcmp(input, "exit")!=0)
        printf("Receive data from %s:%d\n    >%s<\n",
                inet_ntoa(client->sin_addr),
                client->sin_port,
                input);
    close(socket);
}

/*
 *
 */
int main(int argc, char** argv)
{
    printf("Server launched\n");
    struct sigaction act;
    memset(&act, 0, sizeof (act));
    act.sa_handler = &handler;
    act.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &act, NULL) < 0)
    {
        perror("Unable to handle Signal Child!\n");
        exit(1);
    }

    int sd = createTCPSocket();
    struct sockaddr_in *serverAddress = bindTCPAddress(sd, PORT);
    //specify the quene length
    listen(sd, 5);
    struct sockaddr_in clientAddress;
    socklen_t clientLength = sizeof (clientAddress);
    int client = 0;
    printf("Server listening\n");
    while ((client = accept(sd, (struct sockaddr *)&clientAddress, &clientLength)) >= 0)
    {
        printf("Get connection from %s\n", inet_ntoa(clientAddress.sin_addr));
        if(client < 0)
        {
            fprintf(2, "Unable to create child socket\n");
            exit(2);
        }
        if(0 == fork()) // child
        {
            close(sd);
            printf("Child forked for %s:%d\n",inet_ntoa(clientAddress.sin_addr), clientAddress.sin_port);
            childProcess(client, &clientAddress);
            printf("Child terminating\n");
            exit(0);
        }
        close(client);
    }
    free(serverAddress);
    close(sd);
    return (EXIT_SUCCESS);
}
