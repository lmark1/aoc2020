#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <fstream>

namespace aoc_util {

  std::vector<std::string> getLines(const std::string& input_file) {
    std::ifstream file(input_file);
      std::string str; 
      std::vector<std::string> lines;
      while (std::getline(file, str))
      {
        lines.emplace_back(str);
      }
      return lines;
  }
  
}