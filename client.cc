
#include <iostream>
#include "network.hh"
#include <string>

#define N 300

int main(int argc, char **argv)
{
    char buf[N];
    const std::string host = "cpptest.08z.ru";
    const unsigned int port = 12567;
    std::string txt = "asd";

    SocketNet sckt(host, port);

    sckt.readData(buf, N);
    buf[N - 1] = '\0';
    std::cout << buf << std::endl;
    // sckt.sendData((void *)txt.c_str(), txt.size());

    return 0;
}