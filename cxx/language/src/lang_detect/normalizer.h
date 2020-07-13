#pragma once
#include <map>
#include <vector>
#include <string>

namespace langdetect {

class Normalizer {
 public:
  static std::vector<uint32_t> const LATIN1_EXCLUDED;

  static std::vector<uint32_t> const CJK_NORM_TGTS;
  static std::vector<std::vector<uint32_t> > const CJK_NORM_SOURCES;

  static std::vector<uint32_t> const VI_SEARCH_CHARS;
  static std::vector<uint32_t> const VI_REPLACE_CLASS;
  static std::vector<std::vector<uint32_t> > const VI_REPLACE_CHARS;
 private:
  std::map<uint32_t, uint32_t> cjk_map_;
  Normalizer();
  Normalizer(Normalizer const &) = default;
  Normalizer &operator=(Normalizer const &) = default;
 public:
  static Normalizer &instance();
  uint32_t normalize_code(uint32_t const &code);
  void normalize(std::vector<uint32_t> &code);
};

}
