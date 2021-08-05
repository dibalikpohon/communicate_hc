#include <iostream>

#include <asio/io_context.hpp>

#include "communicate/host.hpp"


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << "<port>" << std::endl;
        return 1;
    }

    asio::io_context context;

    std::uint16_t port = strtol(argv[1], nullptr, 10);

    std::fprintf(stdout, "Waiting someone to connect on port %hu...\n", port);
    host h(context, port);

    if (!h) {
        std::cerr << "Failed to wait\n";
        return 2;
    }

    std::fprintf(stdout, "Connected! You're chatting to %s:%hu\nSay hi!\n",
                 h.endpoint_address().c_str(), h.endpoint_port());


    std::string input_string;
    std::thread input_thread(
        [&h, &input_string]
        {
            std::cout << "Ready to send messages...\n";
            while (h) {
                if (!std::getline(std::cin, input_string).eof()) {
                    message m(input_string);
                    h << m;
                } else h.end();
            }
        }
    );

    std::thread output_thread (
        [&h]
        {
            std::cout << "Ready to receive messages...\n";
            while (h) {
                message m;
                if (h.incoming()) {
                    h >> m;
                    std::cout << m.body();
                    std::cout.flush();
                }
            }
        }
    );

    asio::io_context::count_type n =  context.run();
    input_thread.join();
    output_thread.join();

    return 0;
}
