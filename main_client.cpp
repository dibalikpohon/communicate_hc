#include <iostream>

#include <asio/io_context.hpp>

#include "communicate/client.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << "<address> <port>" << std::endl;
        return 1;
    }

    asio::io_context context;

    char* address = argv[1];
    std::uint16_t port = strtol(argv[2], nullptr, 10);

    std::fprintf(stdout, "Trying to connect %s on port %hu...\n", address, port);
    client c(context, address, port);

    if (!c) {
        std::cerr << "Failed to connect!\n";
        return 2;
    }

    std::fprintf(stdout, "Connected! You're chatting to %s:%hu\nSay hi!\n",
                 c.endpoint_address().c_str(), c.endpoint_port());

    std::string input_string;
    std::thread input_thread(
        [&c, &input_string]
        {
            std::cout << "Ready to send messages...\n";
            while (c) {
                if (!std::getline(std::cin, input_string).eof()) {
                    message m(input_string);
                    c << m;
                } else c.end();
            }
        }
    );

    std::thread output_thread (
        [&c]
        {
            std::cout << "Ready to receive messages...\n";
            while (c) {
                message m;
                if (c.incoming()) {
                    c >> m;
                    std::cout << m.body();
                    std::cout.flush();
                }
            }
        }
    );

    context.run();
    input_thread.join();
    output_thread.join();

    return 0;
}
