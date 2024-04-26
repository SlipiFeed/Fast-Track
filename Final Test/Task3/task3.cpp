#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

/***
 * Создайте простой TCP сервер на C++, который принимает сообщения от клиента в циклический буфер и отправляет обратно подтверждение о получении.
*/

int main() 
{
    int server_fd, new_socket, dataSize;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 8080 );

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) 
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while (true) 
    {
        std::cout << "Listening..." << std::endl;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
        {
            perror("accept failed");
            continue; 
        }

        dataSize = read(new_socket, buffer, 1024);

        if (dataSize > 0) 
        {
            buffer[dataSize] = '\0';
            std::cout << "Received message: " << buffer << std::endl;
            const char* response = "Data accepted";
            send(new_socket, response, strlen(response), 0);
        } 
        else if (dataSize == 0)
        {
            std::cout << "Client disconnected" << std::endl;
        } 
        else 
        {
            perror("dataSize less then zero");
        }

        close(new_socket);
    }

    close(server_fd);
    
    return 0;
}