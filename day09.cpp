#include "templates.hpp"
#include "util.hpp"

int part1(const std::string &input, int preamble_size) {
  const auto get_long = [](const auto &el) { return std::stol(el); };
  auto numbers =
      aoc_util::get_numbers<long>(aoc_util::getLines(input), get_long);

  for (int i = preamble_size; i < numbers.size(); i++) {
    std::vector<long> sorted_preamble(numbers.begin() + i - preamble_size,
                                      numbers.begin() + i);
    std::sort(sorted_preamble.begin(), sorted_preamble.end());
    auto val = *(numbers.begin() + i);
    if (!preamble_contains<long>(sorted_preamble.begin(), sorted_preamble.end(),
                                 val)) {
      return val;
    }
  }
  return 0;
}

int main() {
  std::cout << "[part1] test solution: " << part1("input09_1.txt", 5) << "\n";
  std::cout << "[part1] solution: " << part1("input09_2.txt", 25) << "\n";
}