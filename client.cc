
#include <iostream>
#include "network.hh"

#define N 30

int main()
{
    char buf[N];
    SocketNet sckt("127.0.0.1", 22);
    sckt.readData(buf, N);
    buf[N - 1] = '\0';
    std::cout << buf << std::endl;
    return 0;
}