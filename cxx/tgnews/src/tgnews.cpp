#include <iostream>

#include "sysapi.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: tgnews <mode> <arg>" << std::endl;
        return 1;
    }

    --argc; ++argv;
    if (!strcmp(argv[0], "server")) {
        std::string server_binary = "../../server/src/tgserver";
        sysapi::executer(server_binary, server_binary, std::string(argv[1]));
    } else if (!strcmp(argv[0], "languages")) {
    } else if (!strcmp(argv[0], "news")) {
    } else if (!strcmp(argv[0], "categories")) {
    } else if (!strcmp(argv[0], "threads")) {
    } else {
        std::cerr << "Could not recognize mode: '" << argv[0] << "'" << std::endl;
        return 1;
    }

    return 0;
}