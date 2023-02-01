#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define SERVER_MESSAGE_SIZE 1024
#define CLIENT_MESSAGE_SIZE 500
#define PORT 8888

int main(int argc, char *argv[])
{
    int master_socket, new_socket, clientLen, read_size, opt = 1;
    struct sockaddr_in client_address;
    char message[SERVER_MESSAGE_SIZE] = {0};
    char client_message[CLIENT_MESSAGE_SIZE];
    const char *pMessage = "hello aticleworld.com";
   
    //init master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Initialization Failed!");
        exit(EXIT_FAILURE);
    }

    // attaching socket to the port
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        perror("attaching port for the socket failed!");
        exit(EXIT_FAILURE);
    }

    // the type of client IP address is IPv4  (AF_INET)
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;
    client_address.sin_port = htons(PORT);

    if (bind(master_socket, (struct sockaddr*)&client_address, sizeof(client_address)) < 0)
    {
        perror("The binding Failed!");
        exit(EXIT_FAILURE);
    }

    //Listen: 3 connection request will be queued
    if (listen(master_socket, 3) == -1)
    {
        perror("listen failed.");
        return 1;
    }

    printf("Waiting for incoming connections...\n");

    while(1)
    {
        //accept connection from an incoming client
        if ((new_socket = accept(master_socket,(struct sockaddr *)&client_address,(socklen_t*)&clientLen)) < 0)
        {
            perror("accept failed");
            return 1;
        }

        printf("Connection accepted\nClient Ip: %s\tClient Port: %d\n", inet_ntoa(client_address.sin_addr),
        ntohs(client_address.sin_port));
        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);

        //Receive a reply from the client
        if( recv(new_socket, client_message, 500, 0) < 0)
        {
            printf("recv failed");
            break;
        }

        printf("Client reply : %s\n",client_message);
        
        snprintf(message,  strlen(client_message), "Server saw your message: %s", client_message);

        if( send(new_socket, message, strlen(message), 0) < 0)
        {
            printf("Send failed");
            return 1;
        }

        close(new_socket);
    }

    shutdown(master_socket, SHUT_RDWR);
    
    return 0;
}