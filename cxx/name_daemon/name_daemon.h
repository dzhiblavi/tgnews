#ifndef TGSERVER_NAME_DAEMON_H
#define TGSERVER_NAME_DAEMON_H


#include <unordered_map>
#include <filesystem>
#include <chrono>

#define BASE_DAEMON_DIR


struct meta {
    std::filesystem::path json_file;
    uint64_t end;
};


class name_daemon {
    std::unordered_map<std::string, meta> metadata;

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
