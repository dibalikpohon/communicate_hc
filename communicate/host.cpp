#include "host.hpp"

#define DBP_DEBUG__PRINT_EC \
                                else { \
                                std::fprintf(stderr, "Caught an error: "); \
                                std::fputs(ec.message().c_str(), stderr);  \
                                }

using asio::ip::tcp;

host::host(asio::io_context& context_, std::uint16_t port)
    : context(context_),
      socket(context)
{
    tcp::endpoint ep(tcp::v4(), port);
    tcp::acceptor acceptor(context, ep);
    acceptor.accept(socket);
    accept_message();
}

// this function also sus(2)
host& operator>>(host& host_, message& m)
{
    if (host_) {
            m = host_.incoming_message_queue.front();
            host_.incoming_message_queue.pop();
        // }
    }
    return host_;
}

host& operator<<(host& host_, message& m)
{
    auto message_buffer = m.message_buffer();
    asio::async_write(
        host_.socket, asio::buffer((void*)message_buffer->buffer, message_buffer->total_size),
        [message_buffer](const asio::error_code& ec, std::size_t length)
        {
            if (!ec) {
                std::fprintf(stdout, "Wrote %lu bytes.", length);
                std::fflush(stdout);
                delete message_buffer->buffer;
                delete message_buffer;
            }
            DBP_DEBUG__PRINT_EC
        }
    );

    return host_;
}

host::operator bool() const
{
    return socket.is_open();
}

bool host::incoming() const
{
    return !incoming_message_queue.empty();
}

std::string host::endpoint_address()
{
    return socket.remote_endpoint().address().to_string();
}

std::uint16_t host::endpoint_port()
{
    return socket.remote_endpoint().port();
}

void host::end()
{
    socket.close();
}

void host::accept_message()
{
    read_header();
}

void host::read_header()
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

void host::read_body(message::size_type length)
{
    auto* m = new message(length);
    std::string& m_body = m->body();
    asio::async_read(
        socket, asio::buffer((void*)m_body.data(), length),
        [m, length, this](const asio::error_code& ec, std::size_t)
        {
            if (!ec) {
                std::fprintf(stdout, "Read %lu bytes.\n", length);
                message _m(*m);
                incoming_message_queue.push(_m);
                delete m;
                if (*this) read_header();
            } DBP_DEBUG__PRINT_EC
        }
    );
}
