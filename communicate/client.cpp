#include "client.hpp"


#define DBP_DEBUG__PRINT_EC \
                                else { \
                                std::fprintf(stderr, "Caught an error: "); \
                                std::fputs(ec.message().c_str(), stderr);  \
                                }

using asio::ip::tcp;

client::client(asio::io_context& context_, const std::string& address, std::uint16_t port)
    : context(context_),
      socket(context)
{
    tcp::resolver resolver(context);
    tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));
    asio::connect(socket, endpoints);
    accept_message();
}

client& operator>>(client& client_, message& m)
{
    if (client_) {
        m = client_.incoming_message_queue.front();
        client_.incoming_message_queue.pop();
    }
    return client_;
}

client& operator<<(client& client_, message& m)
{
    auto message_buffer = m.message_buffer();
    asio::async_write(
        client_.socket, asio::buffer((void*)message_buffer->buffer, message_buffer->total_size),
        [message_buffer](const asio::error_code& ec, std::size_t length)
        {
            if (!ec) {
                // std::fprintf(stdout, "Wrote %lu bytes.", length);
                // std::fflush(stdout);
                delete message_buffer->buffer;
                delete message_buffer;
            }
            DBP_DEBUG__PRINT_EC
        }
    );

    return client_;
}

client::operator bool() const
{
    return socket.is_open();
}

bool client::incoming() const
{
    return !incoming_message_queue.empty();
}

std::string client::endpoint_address()
{
    return socket.remote_endpoint().address().to_string();
}

std::uint16_t client::endpoint_port()
{
    return socket.remote_endpoint().port();
}

void client::end()
{
    socket.close();
}

void client::accept_message()
{
    read_header();
}

void client::read_header()
{
    auto header_size = new message::size_type;
    asio::async_read(
        socket, asio::buffer(header_size, sizeof(message::size_type)),
        [header_size, this](const asio::error_code& ec, std::size_t)
        {
            message::size_type size = *header_size;
            delete header_size;
            if (!ec) read_body(size);
            DBP_DEBUG__PRINT_EC
        });
}

void client::read_body(message::size_type length)
{
    auto* m = new message(length);
    std::string& m_body = m->body();
    asio::async_read(
        socket, asio::buffer((void*)m_body.data(), length),
        [m, length, this](const asio::error_code& ec, std::size_t)
        {
            if (!ec) {
                // std::fprintf(stdout, "Read %lu bytes.\n", length);
                // std::fflush(stdout);
                message _m(*m);
                incoming_message_queue.push(_m);
                delete m;
                if (*this) read_header();
            } DBP_DEBUG__PRINT_EC
        }
    );
}
