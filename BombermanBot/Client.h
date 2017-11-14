#pragma once

#include <string>
#include <vector>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

#include <boost/beast.hpp>

#include "Map.h"
#include "Command.h"

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

class Client
{
private:
    boost::asio::io_service &_io_service;
    websocket::stream<tcp::socket> _socket;

    Map _map;
public:
    Client(boost::asio::io_service &io_service);

    void run(const std::string &address, uint16_t port, const std::string &user_email, const std::string &user_password,
             const std::function<std::vector<Command>(Client &client, Map &map)> &handler);
};
