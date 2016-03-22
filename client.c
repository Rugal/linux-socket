#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "TCPSocket.h"
#include "Configuration.h"
#include "String.h"


void startClient(int server)
{
    while(1)
    {
        String* s = createString(10);
        char buffer;
        //read from stdin
        while (scanf("%c", &buffer) > 0 && buffer != '\n')
            appendChar(s, buffer);
        if(strncmp("exit", data(s), 4) == 0)
            break;
        //put last newline character
        appendChar(s, buffer);
        //send data to server
        write(server, data(s), size(s)+1);
        deleteString(s);
        //read output from server
        while(read(server, &buffer, 1)>0)
            printf("%c", buffer);
    }
}


/*
 *
 */
int main(int argc, char** argv)
{
    //prepare paremeters
    Configuration* conf = createConfiguration();
    if(argc > 1)
    {
        conf->port = atoi(argv[1]);
    }
    //create socket to connect to server
    int server = createTCPSocket();
    struct sockaddr_in serverAddress ;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(conf->port);
    deleteConfiguration(conf);
    //Trying to connect to server
    printf("Trying to connect server %s:%d\n",inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));
    if (connect(server, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        fprintf(stderr, "Unable to connect to server\n" );
        exit(2);
    }
    printf("Server connected %s:%d\n",inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));
    startClient(server);
    close(server);
    /*exit(0);*/
    return (EXIT_SUCCESS);
}
