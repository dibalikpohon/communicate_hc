#include "safe_queue.hpp"

#include <asio.hpp>

#include "message/message.hpp"

#ifndef COMMUNICATE_HC_CLIENT_HPP
#define COMMUNICATE_HC_CLIENT_HPP

class client
{
public:
    // construct a client class
    client(asio::io_context&, const std::string&, std::uint16_t);

    // grab a message from a queue
    friend client& operator>>(client&, message&);
    // send a message to host
    friend client& operator<<(client&, message&);
    // check if the socket is open
    operator bool() const;
    // check any incoming messages
    bool incoming() const;

    // get the endpoint address
    std::string endpoint_address();
    // get the endpoint port
    std::uint16_t endpoint_port();

    // ends the connection
    void end();

private:
    // self-explanatory
    void accept_message();
    // reads the first 8 byte (of message)
    void read_header();
    // reads the body of message (with specified length)
    void read_body(message::size_type length);

private:
    asio::io_context& context;
    asio::ip::tcp::socket socket;
    // is std::queue<message> enough?
    // or am I implementing thread-safe queue
    // wrong?
    safe_queue<message> incoming_message_queue;

};

#endif //COMMUNICATE_HC_CLIENT_HPP
