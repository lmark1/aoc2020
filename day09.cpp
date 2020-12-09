#include "templates.hpp"
#include "util.hpp"

int part1(const std::string &input, int preamble_size) {
  auto numbers = aoc_util::get_numbers<long>(aoc_util::getLines(input),
                                             aoc_util::get_long);

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

long long part2(const std::string &input, int preamble_size) {
  auto invalid_number = part1(input, preamble_size);
  auto numbers = aoc_util::get_numbers<long long>(aoc_util::getLines(input),
                                                  aoc_util::get_llong);
  std::size_t start_index = 0;
  std::size_t end_index = 0;
  long long local_sum = 0;
  for (std::size_t i = start_index; i < numbers.size(); i++) {
    local_sum += numbers.at(i);

    if (local_sum == invalid_number && end_index != start_index) {
      break;
    }

    if (local_sum < invalid_number) {
      end_index++;
      continue;
    }

    i = ++start_index;
    end_index = start_index;
    local_sum = 0;
  }

  start_index++;
  end_index++;

  std::sort(numbers.begin() + start_index, numbers.begin() + end_index + 1);
  return numbers.at(start_index) + numbers.at(end_index);
}

int main() {
  std::cout << "[part1] test solution: " << part1("input09_1.txt", 5) << "\n";
  std::cout << "[part1] solution: " << part1("input09_2.txt", 25) << "\n";
  std::cout << "[part2] test solution: " << part2("input09_1.txt", 5) << "\n";
  std::cout << "[part2] solution: " << part2("input09_2.txt", 25) << "\n";
}