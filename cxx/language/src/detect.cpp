//
// Created by Роман Агеев on 11.05.2020.
//
#include <iostream>
#include <fstream>
#include "lang_detect/langdetect.h"

using langdetect::Detector;
using std::string;

size_t const MAX_READ_SIZE = 4096;

void message(std::string const &msg, double const &score) {
  std::cout << ": " << msg << ", " << score << std::endl;
}

int main(int argc, char ** argv) {
  std::ifstream file(argv[1]);
  std::string str{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
  Detector detector;
  langdetect::Detected result = detector.detect(str.data(), str.size());
  std::cout << result.name() << std::endl;
}
