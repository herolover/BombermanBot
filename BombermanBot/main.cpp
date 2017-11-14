#include <iostream>

#include "Client.h"

int main(int argc, char *argv[])
{
    boost::asio::io_service io_service;

    if (argc != 5)
    {
        std::cerr << "Usage example:" << std::endl;
        std::cerr << "./BombermanBot.exe <address> <port> <login> <password>" << std::endl;
        return -1;
    }

    Client client(io_service);
    client.run(argv[1], std::stoi(argv[2]), argv[3], argv[4],
               [](Map &map)
    {
        std::vector<Command> commands;

        commands = {Command::PlaceBomb, Command(rand() % 5)};

        return commands;
    });

    io_service.run();

    return 0;
}
