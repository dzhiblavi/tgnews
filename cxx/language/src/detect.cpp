//
// Created by Роман Агеев on 11.05.2020.
//
#include <iostream>
#include <fstream>
#include "lang_detect/langdetect.h"

using langdetect::Detector;
using std::ifstream;
using std::string;

size_t const MAX_READ_SIZE = 4096;

void message(std::string const &msg, double const &score) {
  std::cout << ": " << msg << ", " << score << std::endl;
}

std::string detect(std::string const & input) {
  // here we need to reuse
  Detector detector;
  langdetect::Detected result = detector.detect(input.data(), input.size());
  message(result.name(), result.score());
  return result.name();
}
