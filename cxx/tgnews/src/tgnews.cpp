#include <iostream>

#include "sysapi.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: tgnews <mode> <arg>" << std::endl;
        return 1;
    }

    try {
        std::filesystem::path cur = std::filesystem::path(argv[0]).parent_path();
        --argc, ++argv;
        if (!strcmp(argv[0], "server")) {
            std::filesystem::path server_binary = cur / "../../server/src/tgserver";
            sysapi::executer(server_binary, server_binary, std::string(argv[1]));
        } else if (!strcmp(argv[0], "languages")) {
            std::filesystem::path languages_binary = cur / "../../language/src/language";
            sysapi::executer(languages_binary, languages_binary, std::string(argv[1]));
        } else if (!strcmp(argv[0], "news")) {
        } else if (!strcmp(argv[0], "categories")) {
        } else if (!strcmp(argv[0], "threads")) {
        } else {
            std::cerr << "Could not recognize mode: '" << argv[0] << "'" << std::endl;
            return 1;
        }
    } catch (std::runtime_error const& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "ERROR" << std::endl;
        return 1;
    }

    return 0;
}