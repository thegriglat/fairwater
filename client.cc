
#include <iostream>
#include "network.hh"
#include <string>
#include "datatypes.hh"
#include "response.hh"

using namespace std;

int main(int argc, char **argv)
{
    const std::string host = "cpptest.08z.ru";
    const unsigned int port = 12567;
    SocketNet sckt(host, port);

    // sending start

    sckt.sendStart();
    // getting answer
    Frame frame;
    sckt.readData(&frame, sizeof(frame));
    ResponseParser(&frame, sizeof(frame)).log();

    return 0;
}