#include <iostream>
#include <fstream>
#include <mutex>
#include <memory>
#include <json.h>

#include "lang_detect/langdetect.h"
#include "walker.h"

using langdetect::Detector;
using std::string;

size_t const MAX_READ_SIZE = 4096;

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

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: language <source-dir>" << std::endl;
        return 1;
    }

    std::filesystem::path p(argv[1]);
    std::mutex m;
    std::map<std::string, cvector<std::string>> mp;

    walker w(p, [&](walker::fs_path_t path){
        Detector detector;
        std::ifstream ifs(path);
        std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        std::string result = detector.detect(str.data(), str.size()).name().substr(0, 2);
        std::lock_guard<std::mutex> lg(m);
        mp[result].push_back(path.filename());
    });
    w.run();

    std::vector<json::obj_ptr> jsvec;
    for (auto& p : mp) {
        std::map<std::string, json::obj_ptr> lmp;
        lmp["lang_code"] = json::string::create(p.first);

        std::vector<std::string> v = std::move(p.second.to_vector());
        std::vector<json::obj_ptr> vj(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            vj[i] = json::string::create(v[i]);
        }
        lmp["articles"] = json::array::create(std::move(vj));
        jsvec.push_back(json::map::create(std::move(lmp)));
    }

    std::cout << json::array::create(std::move(jsvec))->to_string() << std::endl;
    return 0;
}
