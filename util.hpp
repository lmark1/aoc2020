#include <algorithm>
#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

namespace aoc_util {

const auto get_long = [](const auto &el) { return std::stol(el); };
const auto get_llong = [](const auto &el) { return std::stol(el); };
const auto get_int = [](const auto &el) { return std::stoi(el); };

template <typename T, typename F>
std::vector<T> get_numbers(const std::vector<std::string> &lines, F &f) {
  std::vector<T> numbers;
  std::transform(lines.begin(), lines.end(), std::back_inserter(numbers),
                 [&](const auto &el) { return f(el); });
  return numbers;
}

template <typename T>
std::vector<std::vector<T>> get_subsets(const std::vector<T> &set) {
  auto n = pow(2, set.size()) - 1;
  std::vector<std::vector<T>> res;
  for (auto i = 0; i < n; i++) {
    auto binary_str = std::bitset<128>(i).to_string();
    std::vector<T> subset;
    auto set_iter = 0;
    for (auto j = binary_str.size() - 1; j >= binary_str.size() - set.size();
         j--) {
      char check = binary_str.at(j);
      if (check == '1') {
        subset.push_back(set.at(set_iter));
      }
      set_iter++;
    }

    if (subset.empty()) {
      continue;
    }
    res.emplace_back(subset);
  }

  res.emplace_back(set);
  return res;
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