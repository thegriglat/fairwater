#ifndef NETWORK_H

#include <netinet/in.h>
#include <string>
#include "datatypes.hh"
#include <vector>

class SocketNet
{
private:
    int socket_fd = 0;
    sockaddr_in address;
    bool isRunning = false;
    Frame readFrame();

public:
    SocketNet(const std::string hostip, const unsigned int port);
    ~SocketNet();
    void Stop();
    int readData(void *buffer, size_t size);
    int sendData(void *buffer, size_t size);

    Frame sendStart();
    Frame sendStop();
    std::vector<Frame> sendGeneralInterrogation();
    Frame sendDigitalControl(uint32_t pointId, uint8_t value);
};

#define NETWORK_H
#endif