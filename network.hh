#ifndef NETWORK_H

#include <netinet/in.h>
#include <string>

class SocketNet
{
private:
    int socket_fd = 0;
    sockaddr_in address;

public:
    SocketNet(const std::string hostip, const unsigned int port);
    ~SocketNet();
    int readData(void *buffer, size_t size);
    int sendData(void *buffer, size_t size);
};

#define NETWORK_H
#endif