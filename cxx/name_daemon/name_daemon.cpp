#include "name_daemon.h"

name_daemon::name_daemon() {
    // restore index from disk
}

name_daemon::~name_daemon() {
    // save index to disk
}

bool name_daemon::compare_time(uint64_t timep) {
    // milli -> just seconds?
    std::chrono::time_point<std::milli> a = std::chrono::steady_clock::now();
    return timep < std::chrono::time_point_cast<uint64_t>(a);
}

bool name_daemon::remove(std::string const &elem) {
    if (contains(elem)) {
        metadata.erase(elem);
        return true;
    }
    return false;
}

bool name_daemon::contains(std::string const &elem) const {
    auto it = metadata.find(elem);
    if (it == metadata.end()) {
        return false;
    }
    return compare_time(it->second.end);
}

std::filesystem::path name_daemon::add(std::string const &elem, uint64_t end_time) {
    if (contains(elem)) {
        return metadata.find(elem)->second.json_file;
    } else {
        // TODO:
        std::filesystem::path store_path;  // decide where to store
        metadata[elem] = {store_path, end_time};
        return store_path;
    }
}

