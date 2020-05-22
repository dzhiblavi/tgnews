#pragma once
#include <unordered_map>
#include <vector>
#include <string>

namespace langdetect {

class NgramStorage {
 public:
  typedef std::vector<double> ProbList;
  typedef std::unordered_map<std::string, ProbList> ProbMap;
 private:
  static std::string const ngramdata_;
  static std::vector<std::string> const langlist_;
  ProbMap probmap_;
  NgramStorage();
  NgramStorage(NgramStorage const &) = default;
  NgramStorage &operator=(NgramStorage const &) = default;
 public:
  static NgramStorage &instance();
  ProbList const &get(std::string const &s);
  bool has(std::string const &s);
  std::string lang_fromindex(size_t const &idx);
  size_t langsize();
  size_t langindex(std::string const &name);
};

}
