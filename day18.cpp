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

struct operation
{
  long long lhs;
  long long rhs;
  char op;

  std::shared_ptr<operation> left_node;
  std::shared_ptr<operation> right_node;
};

long long result_p2(std::string line, std::size_t pos) {
  long long sum = 0;
  
  line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
  std::cout << "\n" << line << "\n";
  std::vector<std::string> split;
  boost::split(split, line, boost::is_any_of("()*"));
  std::vector<std::string> sol;
  for(auto& el : split) {
    
    std::cout << el << " / ";

    if (el.empty() || el == "+") {
      sol.push_back(el);
      continue;
    }

    long long sum = 0;
    std::vector<std::string> adds;
    boost::split(adds, el, boost::is_any_of("+"));
    bool summed = false;
    for (const auto& num : adds) {
      if (num.empty()) {
        continue;
      }
      summed = true;
      sum += std::stoll(num);
    }

    if (adds.front().empty()) {
      sol.emplace_back("+");
    }

    if (summed) {
      sol.push_back(std::to_string(sum));
    }

    if (adds.back().empty()) {
      sol.emplace_back("+");
    }
  }

  std::cout << "\n";
  long long mul = 1LL;
  for (const auto& el: sol) {
    std::cout << el << " / ";
    if (el.empty()) {
      continue;
    }

    //mul *= std::stoll(el);
  }
  return mul;
}

long long part1(const std::string &input) {
  const auto lines = aoc_util::get_lines(input);
  long long sum = 0;
  //std::cout << "\n";
  for (const auto &line : lines) {
    auto res = result(line, 0);
    //std::cout << "Line: " << line << " = " << res.val << "\n";
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
  // std::cout << "[part1] test solution 1: " << part1("input/input18_1.txt") << "\n";
  // std::cout << "[part1] test solution 2: " << part1("input/input18_2.txt") << "\n";
  // std::cout << "[part1] test solution 3: " << part1("input/input18_3.txt") << "\n";
  // std::cout << "[part1] test solution 4: " << part1("input/input18_4.txt") << "\n";
  // std::cout << "[part1] test solution 5: " << part1("input/input18_5.txt") << "\n";
  // std::cout << "[part1] test solution 6: " << part1("input/input18_6.txt") << "\n";
  // std::cout << "[part1] solution: " << part1("input/input18_7.txt") << "\n";
  std::cout << "[part2] test solution 1: " << part2("input/input18_1.txt") << "\n";
  std::cout << "[part2] test solution 2: " << part2("input/input18_2.txt") << "\n";
  std::cout << "[part2] test solution 3: " << part2("input/input18_3.txt") << "\n";
  std::cout << "[part2] test solution 4: " << part2("input/input18_4.txt") << "\n";
  std::cout << "[part2] test solution 5: " << part2("input/input18_5.txt") << "\n";
  std::cout << "[part2] test solution 6: " << part2("input/input18_6.txt") << "\n";
  // std::cout << "[part2] solution: " << part2("input/input18_7.txt") << "\n";
  return 0;
}