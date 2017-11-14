#include "Client.h"

#include <iostream>

#include <boost/asio/buffer.hpp>

Client::Client(boost::asio::io_service &io_service)
    : _io_service(io_service)
    , _socket(io_service)
{
}

void Client::run(const std::string &address, uint16_t port, const std::string &user_email, const std::string &user_password,
                          const std::function<std::vector<Command>(Client &client, Map &map)> &handler)
{
    boost::asio::spawn(_io_service, [this, address, port, user_email, user_password, handler](boost::asio::yield_context yield)
    {
        try
        {
            tcp::endpoint endpoint(boost::asio::ip::address::from_string(address), port);
            _socket.next_layer().async_connect(endpoint, yield);
            _socket.async_handshake(address, "/codenjoy-contest/ws?user=" + user_email + "&pwd=" + user_password, yield);

            while (true)
            {
                boost::beast::flat_buffer buffer;
                _socket.async_read(buffer, yield);

                _map.update(buffer.data());

                _socket.async_write(boost::asio::buffer(commands_to_string(handler(*this, _map))), yield);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    });
}
