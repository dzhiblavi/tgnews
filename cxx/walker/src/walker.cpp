#include "walker.h"

walker::walker(std::string const &path) {
    roots.push(std::filesystem::directory_entry(path));
}

walker::walker(std::vector<std::string> const &paths) {
    for (auto &p : paths) {
        roots.push(std::filesystem::directory_entry(p));
    }
}

void walker::walk() {
    while (!roots.empty()) {
        std::filesystem::directory_entry root = roots.front();
        roots.pop();

        for (auto &p : std::filesystem::directory_iterator(root)) {
            if (p.is_directory()) {
                roots.push(p);
            } else {
                std::cout << p << "\n";
            }
        }
    }
}
