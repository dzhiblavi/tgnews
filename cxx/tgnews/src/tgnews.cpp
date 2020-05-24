#include <iostream>
#include <fstream>

#include "sysapi/sysapi.h"
#include "json/json.h"
#include "language/src/detect.h"
#include "html/parser.h"


void launch_py(nlohmann::json const& js, std::filesystem::path const& cur, std::string const& arg, char** argv) {
    std::filesystem::path tmp_path = std::string("tmp/__tmp_") + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    {
        std::ofstream ofs(cur / tmp_path);
        ofs << js.dump(2);
    }

    std::filesystem::path binary = "/usr/bin/python3";
    std::filesystem::path pyexe = cur / "bin/python/neural.pyc";
    sysapi::executer(binary, binary, pyexe, arg, std::string(tmp_path));
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: tgnews <mode> <arg>" << std::endl;
        return 1;
    }

    try {
        std::filesystem::path cur = std::filesystem::path(argv[0]).parent_path();
        --argc, ++argv;
        if (!strcmp(argv[0], "server")) {
            std::filesystem::path server_binary = cur / "bin/server/src/tgserver";
            sysapi::executer(server_binary, server_binary, std::string(argv[1]));
        } else if (!strcmp(argv[0], "languages")) {
            std::filesystem::path languages_binary = cur / "bin/language/src/language";
            sysapi::executer(languages_binary, languages_binary, std::string(argv[1]));
        } else if (!strcmp(argv[0], "news")) {
            nlohmann::json js = detect(argv[1], {"ru", "en"});
            launch_py(js, cur, argv[0], argv);
        } else if (!strcmp(argv[0], "categories")) {
            nlohmann::json js = detect(argv[1], {"ru", "en"});
            launch_py(js, cur, argv[0], argv);
        } else if (!strcmp(argv[0], "threads")) {
            nlohmann::json js = detect(argv[1], {"ru", "en"},
                                       [] (std::filesystem::path const& p, std::unordered_map<std::string, std::string>& meta) {
                                           std::map<std::string, std::string> mp;
                                           mp["filename"] = p;
                                           mp["published_time"] = std::to_string(html::parser::extract_time(meta["article:published_time"].c_str()));
                                           mp["og:url"] = meta["og:url"];
                                           return mp;
                                       });
            launch_py(js, cur, argv[0], argv);
        } else {
            std::cerr << "Could not recognize mode: '" << argv[0] << "'" << std::endl;
            return 1;
        }
    } catch (std::runtime_error const& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "ERROR" << std::endl;
        return 1;
    }

    return 0;
}