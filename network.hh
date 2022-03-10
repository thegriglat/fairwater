#ifndef NETWORK_H

#include <netinet/in.h>
#include <string>

struct SocketNet
{
    int socket_fd;
    struct sockaddr_in address;
    SocketNet(const std::string hostname, const unsigned int port);
    int readData(void *buffer, size_t size);
    void sendData();
    ~SocketNet();
};

#define NETWORK_H
#endif