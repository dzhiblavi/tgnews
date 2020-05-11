#ifndef TGSERVER_WALKER_H
#define TGSERVER_WALKER_H

#include <string>
#include <filesystem>
#include <iostream>
#include <queue>

#include "thread_pool.h"

class walker {
    thread_pool<8> pool;
    std::queue<std::filesystem::directory_entry> roots;

public:
    explicit walker(std::string const &path);

    explicit walker(std::vector<std::string> const &paths);

    void run();

    static fawait walk(thread_pool<8> &thp, std::filesystem::path path);
};

#endif //TGSERVER_WALKER_H
