#include <iostream>
#include <memory>

#include "detect.h"


using langdetect::Detector;
using std::string;


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: language <source-dir> [languages...]" << std::endl;
        return 1;
    }

    --argc; ++argv;
    std::filesystem::path p = argv[0];
    --argc; ++argv;

    std::set<std::string> langs;
    while (argc > 0) {
        langs.insert(argv[0]);
        --argc; ++argv;
    }

    std::cout << detect(p, langs, true).dump(2) << std::endl;
    return 0;
}
