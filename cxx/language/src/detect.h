#ifndef TGSERVER_DETECT_H
#define TGSERVER_DETECT_H


#include <filesystem>
#include <fstream>
#include <set>

#include "json.h"
#include "walker.h"
#include "lang_detect/langdetect.h"

template <typename T>
class cvector {
    std::vector<T> data;
    std::mutex m;

public:
    void push_back(T const& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_back(value);
    }

    void push_back(T&& value) {
        std::lock_guard<std::mutex> lg(m);
        data.push_back(std::move(value));
    }

    std::vector<T>& to_vector() {
        return data;
    }
};

json::obj_ptr detect(std::filesystem::path const& p, std::set<std::string> const& langs);


#endif //TGSERVER_DETECT_H
