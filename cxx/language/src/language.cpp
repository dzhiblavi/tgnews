#include <iostream>
#include <memory>

#include "detect.h"


using langdetect::Detector;
using std::string;


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: language <source-dir>" << std::endl;
        return 1;
    }

    std::cout << detect(argv[1], std::set<std::string>({"en"}))->to_string() << std::endl;
    return 0;
}
