#ifndef TGSERVER_NAME_DAEMON_H
#define TGSERVER_NAME_DAEMON_H


#include <unordered_map>
#include <filesystem>


#define BASE_DAEMON_DIR


struct meta {
    std::filesystem::path json_file;
    uint64_t end;
};


class name_daemon {
    std::unordered_map<std::string, meta> metadata;

public:
    name_daemon();

    ~name_daemon();
};


#endif //TGSERVER_NAME_DAEMON_H
