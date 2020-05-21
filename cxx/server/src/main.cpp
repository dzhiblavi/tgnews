#include <iostream>

#include "net/io_api.h"
#include "server.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: server <port>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
    try {
        io_api::io_context ctx;
        server srv(ctx, ipv4::endpoint(ipv4::address::any(), port), ipv4::endpoint(ipv4::address::any(), port + 1));
        ctx.exec();
    } catch (std::runtime_error const& e) {
        std::cerr << "main() failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
