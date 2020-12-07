#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <sstream>

namespace aoc_util {

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