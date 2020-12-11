#include "util.hpp"
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

int part1(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  int total_sum = 0;
  std::unordered_set<char> group_q;

  for (const auto &line : lines) {

    if (line.empty()) {
      total_sum += group_q.size();
      group_q.clear();
      continue;
    }

    std::for_each(line.begin(), line.end(),
                  [&](char c) { group_q.emplace(c); });
  }

  total_sum += group_q.size();
  return total_sum;
}

int part2(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  std::unordered_map<char, int> group_q;
  int total_sum = 0;
  int group_size = 0;

  const auto everone_agrees = [&](const auto &el) {
    return el.second == group_size;
  };

  for (const auto &line : lines) {

    if (line.empty()) {
      total_sum += std::count_if(group_q.begin(), group_q.end(), everone_agrees);
      group_size = 0;
      group_q.clear();
      continue;
    }

    group_size++;
    std::for_each(line.begin(), line.end(), [&](const auto c) {
      group_q.emplace(c, 0);
      group_q[c]++;
    });
  }

  total_sum += std::count_if(group_q.begin(), group_q.end(), everone_agrees);
  return total_sum;
}

int main() {
  std::cout << "[part1] test solution: " << part1("input06_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input06_2.txt") << "\n";
  std::cout << "[part2] test solution: " << part2("input06_1.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input06_2.txt") << "\n";
  return 0;
}