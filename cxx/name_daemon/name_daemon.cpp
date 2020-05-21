#include "name_daemon.h"

void name_daemon::load_file(std::filesystem::path &&path) {
    if (std::filesystem::exists(path)) {
        std::ifstream ifs(path);
        std::string data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        nlohmann::json js = nlohmann::json::parse(data);

        for (auto it = js.begin(); it != js.end(); it++) {
            mt[it.key()] = it.value()["time"];
        }
    }
}

bool name_daemon::compare_time(uint64_t timep) {
    // TODO:
    // compare timep with now()
    return true;
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
    std::lock_guard<std::mutex> lg(m);
    std::ofstream fout(METAINFO_FILE);

    nlohmann::json js;

    for (auto const &article : mt) {
        if (compare_time(article.second)) {  // to be optimized
            js[article.first]["time"] = article.second;
        }
    }
    fout << js.dump();
}

bool name_daemon::remove(std::string const &elem) {
    std::lock_guard<std::mutex> lg(m);

    if (contains(elem)) {
        mt.erase(elem);
        return true;
    }
    return false;
}

bool name_daemon::contains(std::string const &elem) {
    std::lock_guard<std::mutex> lg(m);

    auto it = mt.find(elem);
    if (it == mt.end()) {
        return false;
    }
    return compare_time(it->second);
}

bool name_daemon::add(std::string const &elem, uint64_t end_time) {
    std::lock_guard<std::mutex> lg(m);

    if (contains(elem)) {
        return false;
    } else {
        mt[elem] = end_time;
        return true;
    }
}
