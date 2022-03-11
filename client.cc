
#include <iostream>
#include "network.hh"
#include <string>
#include "datatypes.hh"

using namespace std;

int main(int argc, char **argv)
{
    const std::string host = "cpptest.08z.ru";
    const unsigned int port = 12567;
    SocketNet sckt(host, port);

    // sending start

    auto frame = sckt.sendStart();
    frame.print();
    frame = sckt.sendGeneralInterrogation();
    frame.print();
    return 0;
}