#include <string>
#include <cstring>
#include <vector>
#include <memory>

class message
{
public:
    using size_type = std::size_t;
    using message_buffer_type = struct {
        size_type total_size;
        char* buffer;
    };
    using message_buffer_ptr = message_buffer_type*;

    // construct a message with empty body
    message();

    // construct a message
    message(const std::string& str);

    // construct a message, reserves for size
    message(message::size_type length);

    // copy constructor
    message(message const &m);

    // return the length of the body_
    size_type body_length() const;

    // is content empty
    bool empty() const;

    // returns a new message buffer on a heap
    // it's the application responsibility to
    // free it :)
    message_buffer_ptr message_buffer() const;

    std::string& body();

    friend bool operator==(const message&, const message&);

    // destructor
    ~message();

private:
    std::string body_;
};
