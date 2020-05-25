#include "name_daemon.h"

void name_daemon::load_file(std::filesystem::path &&path) {
    if (std::filesystem::exists(path)) {
        std::ifstream ifs(path);
        std::string data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        if (data.empty()) {
            return;
        }
        nlohmann::json js = nlohmann::json::parse(data);

        for (auto it = js.begin(); it != js.end(); it++) {
            uint64_t publ_time = it.value()["published_time"];
            uint64_t end_time = it.value()["end_time"];
            mt[it.key()] = {publ_time, end_time};
        }
    }
}

uint64_t name_daemon::current_time() {
    return std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t name_daemon::max_indexed_time() {
    std::lock_guard<std::recursive_mutex> lg(m);
    return publtimes.front();
}

bool name_daemon::compare_time(uint64_t timep) {
    uint64_t seconds_since_epoch = max_indexed_time();
    return seconds_since_epoch < timep;
}

name_daemon::name_daemon() {
    load_file(METAINFO_FILE);
    std::cout << "DAEMON: Boot" << std::endl;
}

name_daemon::~name_daemon() {
    dump();
    std::cout << "DAEMON: flush" << std::endl;
}

void name_daemon::dump() {
    std::lock_guard<std::recursive_mutex> lg(m);
    std::ofstream fout(METAINFO_FILE);

    nlohmann::json js;
    uint64_t cur_time = max_indexed_time();

    for (auto const &article : mt) {
        if (cur_time < article.second.second) {
            js[article.first]["published_time"] = article.second.first;
            js[article.first]["end_time"] = article.second.second;
        }
    }
    if (!js.empty()) {
        fout << js.dump();
    }
}

bool name_daemon::remove(std::string const &elem) {
    std::lock_guard<std::recursive_mutex> lg(m);

    if (contains(elem)) {
        uint64_t publ_time = mt[elem].first;
        mt.erase(elem);

        del_publtimes.push_back(publ_time);
        std::push_heap(del_publtimes.begin(), del_publtimes.end());

        while (!del_publtimes.empty() && max_indexed_time() == del_publtimes.front()) {
            std::pop_heap(publtimes.begin(), publtimes.end());
            std::pop_heap(del_publtimes.begin(), del_publtimes.end());
            publtimes.pop_back();
            del_publtimes.pop_back();
        }
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
    return compare_time(it->second.second);
}

bool name_daemon::add(std::string const &elem, uint64_t published_time, uint64_t end_time) {
    std::lock_guard<std::recursive_mutex> lg(m);

    if (contains(elem)) {
        return false;
    } else {
        mt[elem] = {published_time, end_time};
        publtimes.push_back(published_time);
        std::push_heap(publtimes.begin(), publtimes.end());
        return true;
    }
}
