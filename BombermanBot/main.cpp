#include <iostream>

#include "Client.h"
#include "Strategy.h"

int main(int argc, char *argv[])
{
    boost::asio::io_service io_service;

    if (argc != 5)
    {
        std::cerr << "Usage example:" << std::endl;
        std::cerr << "./BombermanBot.exe <address> <port> <login> <password>" << std::endl;
        return -1;
    }

    Strategy strategy;

    Client client(io_service);
    client.run(argv[1], std::stoi(argv[2]), argv[3], argv[4], strategy);

    io_service.run();

    return 0;
}
