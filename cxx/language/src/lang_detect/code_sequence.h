//
// Created by Роман Агеев on 11.05.2020.
//
#pragma once
#include <string>
#include <vector>

namespace langdetect {
class CodeSequence {
 public:
  CodeSequence(char const *data, size_t const &length);
  std::vector<std::string> tongram();
 private:
  std::vector<uint32_t> codes_;
  void cleanchar_(std::string &data);
  uint32_t readchar_(std::string const &buffer, size_t &cursor);
  void rough_judge_();
};
}
