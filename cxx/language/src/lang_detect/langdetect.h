#ifndef __LANGDETECT_DETECTOR_H_
#define __LANGDETECT_DETECTOR_H_

#include <string>
#include <vector>

namespace langdetect {

class Detected {
 public:
  Detected(std::string const &name, double const &score);
  Detected();
  bool operator==(Detected const &) const;
  bool operator!=(Detected const &) const;
  bool operator<(Detected const &) const;
  bool operator>(Detected const &) const;
  bool operator<=(Detected const &) const;
  bool operator>=(Detected const &) const;
  void name(std::string const &name);
  std::string const &name() const;
  void score(double const &score);
  double const &score() const;
 private:
  std::string name_;
  double score_;
};

class Detector {
 public:
  static unsigned int const MAX_READ_SIZE;
  static int const DEFAULT_TRIAL;
  static double const DEFAULT_ALPHA;
  static double const ALPHA_WIDTH;
  static int const ITERATION_LIMIT;
  static double const PROB_THRESHOLD;
  static double const CONV_THRESHOLD;
  static int const BASE_FREQ;
  static std::string const UNKNOWN_LANG;

  Detector();
  Detected detect(char const *data, unsigned int const &length);
 private:
  int trial_;
  double alpha_;
  void update_scores_(std::string const &ngram, std::vector<double> &scores, double const &alpha);
  std::string read_ngram_(char const *data, size_t const &len, size_t const &idx, size_t const &num);
  double normalize_(std::vector<double> &scores);
  double random_uniform_();
  double random_gaussian_();
  int random_range_(int const &minnum, int const &maxnum);
};

}

#endif
