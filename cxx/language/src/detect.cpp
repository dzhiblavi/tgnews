#include "detect.h"

nlohmann::json detect(std::filesystem::path const& p, std::set<std::string> const& langs, det_callable_t const& func) {
    std::mutex m;
    std::map<std::string, cvector<std::map<std::string, std::string>>> mp;

    walker w(p, [&](walker::fs_path_t const& path) {
        try {
            langdetect::Detector detector;
            std::ifstream ifs(path);
            std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

            std::unordered_map<std::string, std::string> meta;
            html::parser::extract(str, meta);
            std::string result = detector.detect(str.data(), str.size()).name().substr(0, 2);

            std::lock_guard<std::mutex> lg(m);
            mp[result].push_back(func(path, meta));
        } catch (...) {
            std::cerr << "Detect failed on path: " << path << std::endl;
        }
    });
    w.run();

    nlohmann::json ret;
    for (auto& p : mp) {
        if (!langs.empty() && langs.find(p.first) == langs.end())
            continue;

        ret.push_back({});
        nlohmann::json& cur = ret.back();
        cur["lang_code"] = p.first;
        cur["articles"] = {};
        nlohmann::json& arts = cur["articles"];

        for (auto& s : p.second.to_vector()) {
            if (s.size() == 1) {
                if (s.find("filename") != s.end()) {
                    arts.push_back(std::move(s["filename"]));
                }
            } else {
                if (s.find("filename") != s.end()) {
                    auto name_it = s.find("filename");
                    std::string name = name_it->second;
                    s.erase(name_it);
                    arts[name] = std::move(s);
                }
            }
        }
    }

    return ret;
}

nlohmann::json detect(std::filesystem::path const& p, std::set<std::string> const& langs) {
    return detect(p, langs,
           [] (std::filesystem::path const& p, std::unordered_map<std::string, std::string>& meta) {
               std::map<std::string, std::string> mp;
               mp["filename"] = p;
               return mp;
           });
}
