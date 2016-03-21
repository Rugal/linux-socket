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
#include "Configuration.h"
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

void registerSignal()
{
    struct sigaction act;
    memset(&act, 0, sizeof (act));
    act.sa_handler = &handler;
    act.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &act, NULL) < 0)
    {
        perror("Unable to handle Signal Child!\n");
        exit(1);
    }
}

void start(int socket)
{
    struct sockaddr_in clientAddress;
    socklen_t clientLength = sizeof (clientAddress);
    int client = 0;
    while ((client = accept(socket, (struct sockaddr *)&clientAddress, &clientLength)) >= 0)
    {
        printf("Get connection from %s:%d\n",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
        if(0 == fork()) // child
        {
            close(socket);
            printf("Child forked for %s:%d\n",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
            childProcess(client, &clientAddress);
            printf("Child terminating for %s:%d\n",inet_ntoa(clientAddress.sin_addr),ntohs(clientAddress.sin_port));
            exit(0);
        }
        close(client);
    }
}

/*
 *
 */
int main(int argc, char** argv)
{
    //paremeter setting
    Configuration* conf = createConfiguration();
    if(argc > 1)
    {
        conf->port = atoi(argv[1]);
    }

    //Signal register
    registerSignal();

    //socket setup
    printf("Server launched\n");
    conf->socket = createTCPSocket();
    struct sockaddr_in *serverAddress = bindTCPAddress(conf);
    printf("Bind socket to %s:%d\n",inet_ntoa(serverAddress->sin_addr), ntohs(serverAddress->sin_port));
        //specify the quene length
    listen(conf->socket, conf->queue);

    //start listening
    printf("Server listening\n");
    start(conf->socket);

    //Cleanup
    close(conf->socket);
    deleteConfiguration(conf);
    free(serverAddress);
    return (EXIT_SUCCESS);
}
