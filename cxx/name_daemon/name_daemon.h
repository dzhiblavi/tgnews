#ifndef TGSERVER_NAME_DAEMON_H
#define TGSERVER_NAME_DAEMON_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <list>
#include <mutex>

#include "../walker/src/walker.h"
#include "../json/json.h"


#define BASE_DAEMON_DIR std::filesystem::path("./__daemon")


struct meta {
    std::filesystem::path json_file;
    uint64_t end;
};


class name_daemon {
    static std::list<std::string> categories;
    std::unordered_map<std::string, meta> mt;
    std::mutex m;

private:
    void load_file(std::filesystem::path&& path);

    static void create_directories();

public:
    name_daemon();

    ~name_daemon();
};


#endif //TGSERVER_NAME_DAEMON_H
