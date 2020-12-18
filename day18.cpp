#include "util.hpp"
#include <functional>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

const auto plus_op = [](const auto &a, const auto &b) {
  //std::cout << a << " + " << b << " = " << a + b << "\n";
  return a + b;
};
const auto mul_op = [](const auto &a, const auto &b) {
  //std::cout << a << " * " << b << " = " << a * b << "\n";
  return a * b;
};
std::unordered_map<char, std::function<long long(long long, long long)>> op_map{
    {'+', plus_op}, {'*', mul_op}};

struct res {
  long long val;
  std::size_t pos;
};

res result(const std::string &line, std::size_t pos) {
  long long sum = 0;
  char last_op = '+';
  for (std::size_t i = pos; i < line.size(); i++) {
    const char curr_char = line.at(i);

    if (isspace(curr_char) != 0) {
      continue;
    }

    if (curr_char == '(') {
      auto res = result(line, i + 1);
      sum = op_map[last_op](sum, res.val);
      i = res.pos;
      continue;
    }

    if (curr_char == ')') {
      return {sum, i};
    }

    if (curr_char == '+' || curr_char == '*') {
      last_op = curr_char;
      continue;
    }

    long long val = curr_char - '0';
    sum = op_map[last_op](sum, val);
  }
  return {sum, line.size()};
}

long long result_p2(std::string line, std::size_t pos) {
  line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
  line.insert(0, "(");
  line.insert(line.size(), ")");
  while (!line.empty()) { 
    auto start = line.rfind("(");
    auto end = line.find_first_of(")", start);
    
    if (start > line.size() && end > line.size()) {
      break;
    }

    std::string middle = line.substr(start + 1, end - start - 1);
    std::vector<std::string> split_mul;
    boost::split(split_mul, middle, boost::is_any_of("*"));
    long long mul = 1;
    for (auto& el : split_mul) {
      std::vector<std::string> split_add;
      boost::split(split_add, el, boost::is_any_of("+"));
      long long sum = 0;
      for(auto& sel : split_add) {
        sum += std::stoll(sel);
      }
      mul *= sum;
    }
    
    line.erase(start, end - start + 1);
    line.insert(start, std::to_string(mul));
  }
  return std::stoll(line);
}

long long part1(const std::string &input) {
  const auto lines = aoc_util::get_lines(input);
  long long sum = 0;
  for (const auto &line : lines) {
    auto res = result(line, 0);
    sum += res.val;
  }
  return sum;
}

long long part2(const std::string &input) { 
  const auto lines = aoc_util::get_lines(input);
  long long sum = 0;
  for (const auto& line : lines) {
    sum += result_p2(line, 0);
  }
  return sum; 
}

// clang-format off
int main() {
  std::cout << "[part1] test solution 1: " << part1("input/input18_1.txt") << "\n";
  std::cout << "[part1] test solution 2: " << part1("input/input18_2.txt") << "\n";
  std::cout << "[part1] test solution 3: " << part1("input/input18_3.txt") << "\n";
  std::cout << "[part1] test solution 4: " << part1("input/input18_4.txt") << "\n";
  std::cout << "[part1] test solution 5: " << part1("input/input18_5.txt") << "\n";
  std::cout << "[part1] test solution 6: " << part1("input/input18_6.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input/input18_7.txt") << "\n";
  std::cout << "[part2] test solution 1: " << part2("input/input18_1.txt") << "\n";
  std::cout << "[part2] test solution 2: " << part2("input/input18_2.txt") << "\n";
  std::cout << "[part2] test solution 3: " << part2("input/input18_3.txt") << "\n";
  std::cout << "[part2] test solution 4: " << part2("input/input18_4.txt") << "\n";
  std::cout << "[part2] test solution 5: " << part2("input/input18_5.txt") << "\n";
  std::cout << "[part2] test solution 6: " << part2("input/input18_6.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input/input18_7.txt") << "\n";
  return 0;
}