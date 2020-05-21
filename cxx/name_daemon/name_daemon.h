#ifndef TGSERVER_NAME_DAEMON_H
#define TGSERVER_NAME_DAEMON_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <list>
#include <mutex>
#include <chrono>

#include "../walker/src/walker.h"
#include "../json/json.h"


#define BASE_DAEMON_DIR std::filesystem::path("../__cache")
#define METAINFO_FILE std::filesystem::path("../__cache/META.json")

class name_daemon {
    std::unordered_map<std::string, uint64_t> mt;
    std::mutex m;

private:
    void load_file(std::filesystem::path&& path);

private:
    static uint64_t current_time();

    static bool compare_time(uint64_t);

public:
    name_daemon();

    ~name_daemon();

    void dump();

    bool remove(std::string const&);

    bool contains(std::string const&);

    bool add(std::string const&, uint64_t);
};


#endif //TGSERVER_NAME_DAEMON_H
