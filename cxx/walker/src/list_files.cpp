#include "walker.h"

#include <vector>

int main(int argc, char *argv[]) {
    const char *usage = "Usage: walker file_path";
    argc--, argv++;
    if (argc < 1) {
        std::cerr << usage;
        return -1;
    }

    std::vector<std::string> starts;
    while (argc) {
        starts.emplace_back(argv[0]);
        argc--, argv++;
    }

    walker w(starts, [](walker::fs_path_t path){
        std::cout << path.string() << std::endl;
    });
    w.run();
}