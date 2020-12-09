#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace aoc_util {

const auto get_long = [](const auto &el) { return std::stol(el); };
const auto get_llong = [](const auto &el) { return std::stol(el); };
const auto get_int = [](const auto &el) { return std::stoi(el); };

template<typename T, typename F>
std::vector<T> get_numbers(const std::vector<std::string>& lines, F& f)
{
  std::vector<T> numbers;
  std::transform(lines.begin(), lines.end(), std::back_inserter(numbers),
                 [&](const auto &el) { return f(el); });
  return numbers;
}

std::vector<std::string> getLines(const std::string &input_file) {
  std::ifstream file(input_file);
  std::string str;
  std::vector<std::string> lines;
  while (std::getline(file, str)) {
    lines.emplace_back(str);
  }
  return lines;
}

std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

} // namespace aoc_util