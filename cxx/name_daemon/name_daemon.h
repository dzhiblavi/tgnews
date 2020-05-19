#ifndef TGSERVER_NAME_DAEMON_H
#define TGSERVER_NAME_DAEMON_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
<<<<<<< HEAD
#include <fstream>
#include <list>
#include <mutex>

#include "../walker/src/walker.h"
#include "../json/json.h"

=======
#include <chrono>
>>>>>>> 4ef66bedbe8aa24ef9818b3df6f19f27dd411269

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

private:
    static bool compare_time(uint64_t);

public:
    name_daemon();

    ~name_daemon();

    bool remove(std::string const&);

    bool contains(std::string const&) const;

    std::filesystem::path add(std::string const&, uint64_t);
};


#endif //TGSERVER_NAME_DAEMON_H
