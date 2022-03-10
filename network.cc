#include "network.hh"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <memory.h>

#define IP_PROTOCOL (0)

namespace
{

    std::string getIPByName(const std::string &hostname)
    {
        struct hostent *he;
        struct in_addr **addr_list;
        int i;

        if ((he = gethostbyname(hostname.c_str())) == NULL)
        {
            // get the host info
            std::cerr << "Cannot get IP for " << hostname << std::endl;
            return "";
        }

        addr_list = (struct in_addr **)he->h_addr_list;

        std::string ip = inet_ntoa(*addr_list[0]);
        return ip;
    }
};

SocketNet::SocketNet(const std::string hostname, const unsigned int port)
{
    std::cout << "Connecting to " << hostname << ":" << port << std::endl;
    socket_fd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL);
    if (socket_fd < 0)
    {
        std::cerr << "Cannot create socket!" << std::endl;
        exit(1);
    }
    // memset(&address, '0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    // TODO: hostname is IP !!!
    std::cout << getIPByName(hostname) << std::endl;
    if (inet_pton(AF_INET, getIPByName(hostname).c_str(), &address.sin_addr) <= 0)
    {
        std::cerr << "inet_pton failed" << std::endl;
        exit(1);
    }
    if (connect(socket_fd, (sockaddr *)(&address), sizeof(address)) < 0)
    {
        std::cerr << "Socket connect failed!" << std::endl;
        exit(1);
    }
    std::cout << "Connected" << std::endl;
};

SocketNet::~SocketNet()
{
    shutdown(socket_fd, 2);
}

int SocketNet::readData(void *buffer, size_t size)
{
    // memset(buffer, '0', size);
    const int r = read(socket_fd, buffer, size - 1);
    if (r < 0)
    {
        std::cerr << "Cannot read data from socket" << std::endl;
    }
    return r;
}

int SocketNet::sendData(void *buffer, size_t size)
{
    const int r = send(socket_fd, buffer, size - 1, 0);
    if (r < 0)
    {
        std::cerr << "Cannot send data to socket" << std::endl;
    }
    return r;
}