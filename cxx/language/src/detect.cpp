#include "detect.h"

json::obj_ptr detect(std::filesystem::path const& p, std::set<std::string> const& langs) {
    std::mutex m;
    std::map<std::string, cvector<std::string>> mp;

    walker w(p, [&](walker::fs_path_t const& path) {
        langdetect::Detector detector;
        std::ifstream ifs(path);
        std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        std::string result = detector.detect(str.data(), str.size()).name().substr(0, 2);
        std::lock_guard<std::mutex> lg(m);
        mp[result].push_back(path.filename());
    });
    w.run();

    std::vector<json::obj_ptr> jsvec;
    for (auto& p : mp) {
        if (!langs.empty() && langs.find(p.first) == langs.end())
            continue;

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

    return json::array::create(std::move(jsvec));
}
