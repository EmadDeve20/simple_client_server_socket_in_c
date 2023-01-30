#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define SERVER_MESSAGE_SIZE 1024
#define CLIENT_MESSAGE_SIZE 500

int create_socket()
{
    int sock;
    puts("Create the socket\n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    return sock;
}

int bind_socket(int sock)
{
    int iRetval=-1;
    int ClientPort = 8888;
    struct sockaddr_in  remote= {0};
    // Internet address family
    remote.sin_family = AF_INET;
    // Any incoming interface
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    iRetval = bind(sock,(struct sockaddr *)&remote,sizeof(remote));
    return iRetval;
}

int main(int argc, char *argv[])
{
    int socket_desc, sock, clientLen, read_size;
    struct sockaddr_in client_address;
    char message[SERVER_MESSAGE_SIZE] = {0};
    char client_message[CLIENT_MESSAGE_SIZE];
    const char *pMessage = "hello aticleworld.com";
    //Create socket
    socket_desc = create_socket();
    if (socket_desc == -1)
    {
        printf("Could not create socket");
        return 1;
    }
    //Bind
    if( bind_socket(socket_desc) < 0)
    {
        //print the error message
        perror("bind failed.");
        return 1;
    }
    //Listen
    if (listen(socket_desc, 3) == -1)
    {
        perror("listen failed.");
        return 1;
    }
    //Accept and incoming connection
    while(1)
    {
        printf("Waiting for incoming connections...\n");
        clientLen = sizeof(struct sockaddr_in);
        //accept connection from an incoming client
        sock = accept(socket_desc,(struct sockaddr *)&client_address,(socklen_t*)&clientLen);
        if (sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        printf("Connection accepted\nClient Ip: %s\tClient Port: %d\n", inet_ntoa(client_address.sin_addr),
        ntohs(client_address.sin_port));
        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);

        //Receive a reply from the client
        if( recv(sock, client_message, 500, 0) < 0)
        {
            printf("recv failed");
            break;
        }

        printf("Client reply : %s\n",client_message);
        
        snprintf(message,  strlen(client_message), "Server saw your message: %s", client_message);

        if( send(sock, message, strlen(message), 0) < 0)
        {
            printf("Send failed");
            return 1;
        }

        close(sock);
        sleep(1);
    }
    return 0;
}