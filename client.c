#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "TCPSocket.h"
#include "Configuration.h"

/*
 *
 */
int main(int argc, char** argv)
{
    Configuration* conf = createConfiguration();
    if(argc > 1)
    {
        conf->port = atoi(argv[1]);
    }
    int server = createTCPSocket();
    struct sockaddr_in serverAddress ;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(conf->port);
    //specify the quene length
    printf("Trying to connect server %s:%d\n",inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));
    if (connect(server, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        fprintf(stderr, "Unable to connect to server\n" );
        exit(2);
    }
    printf("Server connected %s:%d\n",inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));
    char input[1024];
    while(scanf("%s", input) && strcmp(input, "exit")!=0)
    {
        write(server, input, strlen(input)+1);
        char buffer;
        while(read(server, buffer, 1)>0)
            printf("%c", buffer);
    }
    close(server);
    deleteConfiguration(conf);
    exit(0);
    return (EXIT_SUCCESS);
}
