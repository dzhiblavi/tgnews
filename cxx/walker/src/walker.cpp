#include "walker.h"

walker::walker(std::string const &path, file_handler_t func) : fh(func) {
    roots.push_back(path);
}

walker::walker(std::vector<std::string> const &paths, file_handler_t func) : fh(func) {
    roots = paths;
}

void walker::run() {
    while (!roots.empty()) {
        auto p = roots.back();
        roots.pop_back();
        walker::walk(pool, fh, p);
    }
    pool.await();
}

fawait walker::walk(thp_t &thp, file_handler_t &fh, fs_path_t const &path) {
    return thp.submit([&thp, &fh, path] {
        try {
            for (auto &p : std::filesystem::directory_iterator(path)) {
                if (p.is_directory()) {
                    walker::walk(thp, fh, p.path());
                } else {
                    fh(p.path());
                }
            }
        } catch (std::filesystem::filesystem_error const &er) {
            std::cerr << "ERROR: " << er.what() << std::endl;
        } catch (...) {
            std::cerr << "ERROR" << std::endl;
        }
    });
}
