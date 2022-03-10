
#include <iostream>
#include "network.hh"

int main()
{
    char buf[100];
    SocketNet sckt("127.0.0.1", 22);
    sckt.readData(buf, 100);
    std::cout << buf << std::endl;
    return 0;
}