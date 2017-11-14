#include "Client.h"

int main()
{
    boost::asio::io_service io_service;

    Client client(io_service);
    client.run("52.232.32.105", 8080, "anton.zhv@gmail.com", "1q2w3e4r",
               [](Client &client, Map &map)
    {
        std::vector<Command> commands;

        commands = {Command::PlaceBomb, Command(rand() % 5)};

        return commands;
    });

    io_service.run();

    return 0;
}
