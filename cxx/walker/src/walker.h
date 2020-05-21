#ifndef TGSERVER_WALKER_H
#define TGSERVER_WALKER_H

#include <string>
#include <filesystem>
#include <functional>
#include <iostream>
#include <queue>

#include "sysapi/thread_pool.h"

class walker {
public:
    static const size_t pool_size = 8;
    using fs_path_t = std::filesystem::path;
    using thp_t = thread_pool<pool_size>;
    using file_handler_t = std::function<void(fs_path_t)>;

private:
    thp_t poolDir;
    thp_t poolFile;
    file_handler_t fh;
    std::vector<std::string> roots;

public:
    walker(std::string const &path, file_handler_t);

    walker(std::vector<std::string> const &paths, file_handler_t);

    void run();

    static fawait walk(thp_t &poolDir, thp_t& poolFile, file_handler_t &fh, fs_path_t const &path);
};

#endif //TGSERVER_WALKER_H
