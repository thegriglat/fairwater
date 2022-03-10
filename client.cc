
#include <iostream>
#include "network.hh"

int main()
{
    char buf[20];
    SocketNet sckt("127.0.0.1", 22);
    sckt.readData(buf, 20);
    std::cout << buf << std::endl;
    return 0;
}