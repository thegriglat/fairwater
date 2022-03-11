#ifndef NETWORK_H

#include <netinet/in.h>
#include <string>
#include "datatypes.hh"

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

    void sendStart();
    void sendStop();
    void sendGeneralInterrogation();
    void sendDigitalControl(uint8_t pointId, uint8_t value);

    Frame readFrame();
};

#define NETWORK_H
#endif