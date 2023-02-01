#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_MESSAGE_SIZE 1024
#define CLIENT_MESSAGE_SIZE 500

int main()
{

    char server_ip[16] = "127.0.0.1"; 
    char message[CLIENT_MESSAGE_SIZE], response[SERVER_MESSAGE_SIZE];
    int server_port = 8888;
    int socket_port, client_fd;
    struct sockaddr_in sock_addr;
    

    // setup socket for IPv4
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(server_port);

    // Convert IPv4 from text to binary
    if (inet_pton(AF_INET, server_ip, &sock_addr.sin_addr) <= 0)
    {
        perror("Invalid address or Address not supported\n");
        exit(EXIT_FAILURE);
    }

    // create connection(master) socket
    if ((socket_port = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Connection Error!\n");
        exit(EXIT_FAILURE);
    }

    // create client socket
    if ((client_fd = connect(socket_port, (struct sockaddr*)&sock_addr, sizeof(sock_addr))) < 0)
    {
        perror("Connection Faild!\n");
        exit(EXIT_FAILURE);
    }

    printf("Your Connection To Server Successful!\n");
    printf("Write Your Message for Server: ");

    fgets(message, 500, stdin);

    // send your message to server 
    send(client_fd, message, CLIENT_MESSAGE_SIZE, 0);

    // recive message for server (get response)
    recv(client_fd, response, SERVER_MESSAGE_SIZE, 0);

    printf("Message From Server: %s\n", response);

    close(client_fd);
    shutdown(socket_port, SHUT_RDWR);
}