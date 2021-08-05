#include "message.hpp"

message::message() { }

message::message(const std::string& str)
{
    body_ = str;
}

message::message(message::size_type sz)
{
    body_.resize(sz);
}

message::message(message const &m)
{
    body_ = std::string(m.body_);
}

message::size_type message::body_length() const
{
    return body_.length();
}

message::message_buffer_ptr message::message_buffer() const
{
    auto buffer = new message_buffer_type();
    auto length = body_length();
    buffer->total_size = sizeof(message::size_type) + length;
    buffer->buffer = new char[buffer->total_size];
    std::memmove(buffer->buffer, &length, sizeof(message::size_type));
    std::memmove(buffer->buffer + sizeof(message::size_type), body_.data(), length);

    return buffer;
}

std::string& message::body()
{
    return body_;
}

message::~message()
{
    body_.clear();
}

bool message::empty() const
{
    return body_.empty();
}

bool operator==(const message& a, const message& other)
{
    return a.body_ == other.body_;
}
