#include "name_daemon.h"

void name_daemon::load_file(std::filesystem::path &&path) {
    if (std::filesystem::exists(path)) {
        std::ifstream ifs(path);
        std::string data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        if (data.empty()) {
            return;
        }
        nlohmann::json js = nlohmann::json::parse(data);
        uint64_t cur_time = current_time();

        for (auto it = js.begin(); it != js.end(); it++) {
            uint64_t art_time = it.value()["time"];
            if (cur_time < art_time) {
                mt[it.key()] = art_time;
            }
        }
    }
}

uint64_t name_daemon::current_time() {
    return std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
}

bool name_daemon::compare_time(uint64_t timep) {
    uint64_t seconds_since_epoch = current_time();
    return seconds_since_epoch < timep;
}

name_daemon::name_daemon() {
    std::cerr << "DAEMON: Boot" << std::endl;
    load_file(METAINFO_FILE);
    std::cerr << "DAEMON: OK" << std::endl;
}

name_daemon::~name_daemon() {
    dump();
}

void name_daemon::dump() {
    std::lock_guard<std::recursive_mutex> lg(m);
    std::ofstream fout(METAINFO_FILE);

    nlohmann::json js;
    uint64_t cur_time = current_time();

    for (auto const &article : mt) {
        if (cur_time < article.second) {
            js[article.first]["time"] = article.second;
        }
    }
    if (!js.empty()) {
        fout << js.dump();
    }
}

bool name_daemon::remove(std::string const &elem) {
    std::lock_guard<std::recursive_mutex> lg(m);

    if (contains(elem)) {
        mt.erase(elem);
        return true;
    }
    return false;
}

bool name_daemon::contains(std::string const &elem) {
    std::lock_guard<std::recursive_mutex> lg(m);

    auto it = mt.find(elem);
    if (it == mt.end()) {
        return false;
    }
    return compare_time(it->second);
}

bool name_daemon::add(std::string const &elem, uint64_t end_time) {
    std::lock_guard<std::recursive_mutex> lg(m);

    if (contains(elem)) {
        return false;
    } else {
        mt[elem] = end_time;
        return true;
    }
}
