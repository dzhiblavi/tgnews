#include "name_daemon.h"

std::list<std::string> name_daemon::categories = {
        "society",
        "economy",
        "technology",
        "sports",
        "entertainment",
        "science",
        "other",
};

void name_daemon::load_file(std::filesystem::path&& path) {
    std::ifstream ifs(path);
    std::string data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    nlohmann::json js = nlohmann::json::parse(data);

    std::list<meta> ml;
    for (auto const& x : js) {
        ml.push_back({});
        ml.back().json_file = path / x["name"];
    }

    std::lock_guard<std::mutex> lg(m);
    for (auto&& mtt : ml) {
        mt[mtt.json_file] = std::move(mtt);
    }
}

void name_daemon::create_directories() {
    std::filesystem::create_directories(BASE_DAEMON_DIR / "ru/news");
    std::filesystem::create_directories(BASE_DAEMON_DIR / "ru/notnews");
    std::filesystem::create_directories(BASE_DAEMON_DIR / "en/news");
    std::filesystem::create_directories(BASE_DAEMON_DIR / "en/notnews");

    for (auto const& s : categories) {
        std::filesystem::create_directories(BASE_DAEMON_DIR / "ru/news" / s);
        std::filesystem::create_directories(BASE_DAEMON_DIR / "en/news" / s);
    }
}

name_daemon::name_daemon() {
    std::cerr << "DAEMON: Boot" << std::endl;
    if (std::filesystem::is_directory(BASE_DAEMON_DIR)) {
        std::cerr << "DAEMON: Load" << std::endl;
        walker w(BASE_DAEMON_DIR, [&] (std::filesystem::path&& p) {
            load_file(std::move(p));
        });
        w.run();
    } else {
        create_directories();
    }
    std::cerr << "DAEMON: OK" << std::endl;
}

name_daemon::~name_daemon() {
    // ignore
}

bool name_daemon::compare_time(uint64_t timep) {
    // milli -> just seconds?
    // compare timep with now()
    return false;
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
    return compare_time(it->second.end);
}

std::filesystem::path name_daemon::add(std::string const &elem, uint64_t end_time) {
    std::lock_guard<std::mutex> lg(m);

    if (contains(elem)) {
        return mt.find(elem)->second.json_file;
    } else {
        // TODO:
        std::filesystem::path store_path;  // decide where to store
        mt[elem] = {store_path, end_time};
        return store_path;
    }
}
