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
#include <algorithm>

#include "../walker/src/walker.h"
#include "../json/json.h"


#define METAINFO_FILE std::filesystem::path("tmp/META.json")


class name_daemon {
    std::unordered_map<std::string, std::pair<uint64_t, uint64_t>> mt;
    std::recursive_mutex m;
    std::vector<uint64_t> publtimes = {0};
    std::vector<uint64_t> del_publtimes = {};

private:
    void load_file(std::filesystem::path&& path);

public:
    name_daemon();

    ~name_daemon();

    void dump();

    bool remove(std::string const&);

    bool contains(std::string const&);

    bool add(std::string const&, uint64_t published_time, uint64_t end_time);

    uint64_t current_time();

    bool compare_time(uint64_t);
};


#endif //TGSERVER_NAME_DAEMON_H
