#include "walker.h"

#include <vector>
#include <mutex>

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

    std::mutex coutm;
    walker w(starts, [&](const walker::fs_path_t& path){
        std::lock_guard<std::mutex> lg(coutm);
        std::cout << path.string() << std::endl;
    });
    w.run();

    return 0;
}