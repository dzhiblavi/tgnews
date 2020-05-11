#include "walker.h"

walker::walker(std::string const &path) {
    roots.push(std::filesystem::directory_entry(path));
}

walker::walker(std::vector<std::string> const &paths) {
    for (auto &p : paths) {
        roots.push(std::filesystem::directory_entry(p));
    }
}

void walker::run() {
    while (!roots.empty()) {
        auto p = roots.front();
        roots.pop();
        walker::walk(pool, p);
    }
    pool.await();
}

fawait walker::walk(thread_pool<8> &thp, std::filesystem::path path) {
    std::cout << "HERE: " << path << std::endl;
    return thp.submit([&thp, path] {
        try {
            std::cout << "PROCESS " << path << '\n';
            for (auto p : std::filesystem::directory_iterator(path)) {
                if (p.is_directory()) {
                    std::cout << "DIR " << p.path() << '\n';
                    walker::walk(thp, p.path());
                    std::cout << "LOAD";
                } else {
                    std::cout << p.path() << "\n";
                }
            }
        } catch (std::filesystem::filesystem_error const& er) {
            std::cerr << "ERROR: " << er.what() << std::endl;
        } catch (...) {
            std::cerr << "ERROR" << std::endl;
        }
    });
}