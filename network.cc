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
#include <vector>
#include "datatypes.hh"

#define IP_PROTOCOL (0)

using namespace std;

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

    string FrameType2Str(FrameType type)
    {
        switch (type)
        {
        case FrameType::Ack:
            return "Ack";
        case FrameType::Nack:
            return "Nack";
        case FrameType::DigitalPoints:
            return "DigitalPoints";
        case FrameType::AnalogPoints:
            return "AnalogPoints";
        }
        return "undefined type";
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
    isRunning = true;
};

SocketNet::~SocketNet()
{
    Stop();
}

void SocketNet::Stop()
{
    if (!isRunning)
        return;
    isRunning = false;
    std::cerr << "Connection closed." << std::endl;
    shutdown(socket_fd, 2);
}

int SocketNet::readData(void *buffer, size_t size)
{
    // memset(buffer, '0', size);
    const int r = read(socket_fd, buffer, size);
    if (r < 0)
    {
        std::cerr << "Cannot read data from socket: " << r << std::endl;
    }
    return r;
}

int SocketNet::sendData(void *buffer, size_t size)
{
    const int r = write(socket_fd, buffer, size);
    if (r < 0)
    {
        std::cerr << "Cannot send data to socket: " << r << std::endl;
    }
    return r;
}

Frame SocketNet::sendStart()
{
    cout << "Sending Start" << endl;
    Frame frame;
    frame.header.frameType = FrameType::Start;
    frame.header.length = sizeof(frame);
    sendData(&frame, sizeof(frame));
    return readFrame();
}

Frame SocketNet::sendStop()
{
    cout << "Sending Stop" << endl;
    Frame frame;
    frame.header.frameType = FrameType::Stop;
    frame.header.length = sizeof(frame);
    sendData(&frame, sizeof(frame));
    auto f = readFrame();
    if (f.header.frameType == FrameType::Ack)
    {
        Stop();
    }
    return f;
}

std::vector<Frame> SocketNet::sendGeneralInterrogation()
{
    cout << "Sending GeneralInterrogation" << endl;
    Frame frame;
    frame.header.frameType = FrameType::GeneralInterrogation;
    frame.header.length = sizeof(frame);
    sendData(&frame, sizeof(frame));
    auto readed_frame = readFrame();
    std::vector<Frame> frames;
    if (readed_frame.header.frameType == FrameType::Ack)
    {
        frames.push_back(readFrame()); // digital
        frames.push_back(readFrame()); // analog
    }
    return frames;
}

Frame SocketNet::sendDigitalControl(uint32_t pointId, uint8_t value)
{
    cout << "Sending DigitalControl\n  pointId: " << pointId << ", value: " << (int)value << endl;
    Frame frame;
    frame.header.frameType = FrameType::DigitalControl;
    frame.header.length = sizeof(frame) + sizeof(pointId) + sizeof(value);
    sendData(&frame, sizeof(frame));
    sendData(&pointId, sizeof(pointId));
    sendData(&value, sizeof(value));
    if (!isRunning)
        return frame;
    auto f = readFrame();
    if (f.header.frameType == FrameType::Ack)
    {
        return readFrame();
    }
    return f;
}

Frame SocketNet::readFrame()
{
    Frame frame;
    readData(&frame.header, sizeof(frame.header));
    cout << "Received " << FrameType2Str(frame.header.frameType) << endl;
    if (frame.header.frameType == FrameType::DigitalPoints)
    {
        readData(&(frame.payload_header), sizeof(frame.payload_header));

        if (frame.payload_header.count != 0)
        {
            frame.digital_points = new DigitalPoint[frame.payload_header.count];
            readData(frame.digital_points, frame.payload_header.count * sizeof(DigitalPoint));
        }
    }
    if (frame.header.frameType == FrameType::AnalogPoints)
    {
        readData(&frame.payload_header, sizeof(frame.payload_header));
        if (frame.payload_header.count != 0)
        {
            frame.analog_points = new AnalogPoint[frame.payload_header.count];
            readData(frame.analog_points, frame.payload_header.count * sizeof(AnalogPoint));
        }
    }
    return frame;
}