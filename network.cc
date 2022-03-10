#include "network.hh"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <unistd.h>

#define IP_PROTOCOL (0)

SocketNet::SocketNet(const std::string hostname, const unsigned int port)
{
    if (socket_fd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL) < 0)
    {
        std::cerr << "Cannot create socket" << std::endl;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    // TODO: hostname is IP
    if (inet_pton(AF_INET, hostname.c_str(), &address.sin_addr) <= 0)
    {
        printf("\n inet_pton error occured\n");
        std::cerr << "inet_pton failed" << std::endl;
        exit(1);
    }
    if (connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Socket connect failed!" << std::endl;
        exit(1);
    }
};

SocketNet::~SocketNet()
{
    shutdown(socket_fd, 2);
}

int SocketNet::readData(void *buffer, size_t size)
{
    return read(socket_fd, buffer, size - 1);
}