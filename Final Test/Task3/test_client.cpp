#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    const char* message = "Hello from client!";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    send(sock, message, strlen(message), 0);
    std::cout << "Message sent\n";

    valread = read(sock, buffer, 1024);
    std::cout << "Server: " << buffer << std::endl;


    close(sock);

    return 0;
}