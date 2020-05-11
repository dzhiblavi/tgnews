#ifndef TGSERVER_WALKER_H
#define TGSERVER_WALKER_H

#include <string>
#include <filesystem>
#include <iostream>
#include <queue>

class walker {
    std::queue<std::filesystem::directory_entry> roots;

public:
    explicit walker(std::string const &path);

    explicit walker(std::vector<std::string> const &paths);

    void walk();
};

#endif //TGSERVER_WALKER_H
