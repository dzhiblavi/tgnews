#include "detect.h"

nlohmann::json detect(std::filesystem::path const& p, std::set<std::string> const& langs, bool filenames_only) {
    std::mutex m;
    std::map<std::string, cvector<std::string>> mp;

    walker w(p, [&](walker::fs_path_t const& path) {
        langdetect::Detector detector;
        std::ifstream ifs(path);
        std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        std::string result = detector.detect(str.data(), str.size()).name().substr(0, 2);
        std::lock_guard<std::mutex> lg(m);
        mp[result].push_back(filenames_only ? path.filename() : path);
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

        for (std::string& s : p.second.to_vector()) {
            arts.push_back(std::move(s));
        }
    }

    return ret;
}
