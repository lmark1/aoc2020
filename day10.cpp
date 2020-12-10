#include "util.hpp"

int part1(const std::string &input) {
  auto numbers =
      aoc_util::get_numbers<int>(aoc_util::getLines(input), aoc_util::get_int);
  std::sort(numbers.begin(), numbers.end());
  int diff_three = 0;
  int diff_one = 0;
  int last_adapter = 0;
  for (const auto &number : numbers) {
    auto curr_diff = number - last_adapter;
    if (curr_diff == 1) {
      diff_one++;
    } else if (curr_diff == 3) {
      diff_three++;
    }
    last_adapter = number;
  }
  diff_three++;
  return diff_one * diff_three;
}

int arrangement_count(const std::vector<int> &numbers,
                      std::size_t current_index, int last_adapter) {
  // If we are at the end
  if (numbers.size() <= current_index) {
    return 1;
  }

  int count = 0;
  for (auto i = current_index; i < numbers.size(); i++) {
    auto diff = numbers.at(i) - last_adapter;
    if (diff > 3) {
      break;
    }
    count += arrangement_count(numbers, i + 1, numbers.at(i));
  }
  return count;
}

long long smart_arrangement_count(std::vector<int> &numbers) {
  long long count = 1;
  std::vector<int> adapter_set;
  adapter_set.emplace_back(0);
  int last = -1;
  numbers.insert(numbers.begin(), 0);
  for (int i = 0; i < numbers.size(); i++) {
    auto curr_number = numbers.at(i);
    auto first_invalid_it =
        std::find_if(numbers.begin() + i, numbers.end(),
                     [&](const auto &el) { return el - curr_number > 3; });
    auto invalid_index = first_invalid_it - numbers.begin();
    auto invalid_size = invalid_index - i;

    if (invalid_size < 3) {
      continue;
    }

    std::vector<int> adapter_set(numbers.begin() + i,
                                 numbers.begin() + i + invalid_size);
    auto subsets = aoc_util::get_subsets(adapter_set);
    long long valid_subsets = 0;
    for (const auto &subset : subsets) {

      // Case where there is a 0 subset
      if (adapter_set.front() == 0 && subset.front() != 0) {
        continue;
      }

      // Lower bound check
      if (i - 1 >= 0) {
        auto lower_bound = numbers.at(i - 1);
        auto subset_lower = subset.front();
        if (subset_lower - lower_bound > 3) {
          continue;
        }
      }

      // Upper bound check
      if (i + invalid_size < numbers.size()) {
        auto upper_bound = numbers.at(i + invalid_size);
        auto subset_upper = subset.back();
        if (upper_bound - subset_upper > 3) {
          continue;
        }
      }

      // The subset is valid
      valid_subsets++;
    }

    count *= valid_subsets;
    i += invalid_size - 1;
  }
  return count;
}

long long part2(const std::string &input) {
  auto numbers =
      aoc_util::get_numbers<int>(aoc_util::getLines(input), aoc_util::get_int);
  std::sort(numbers.begin(), numbers.end());
  return smart_arrangement_count(numbers);
}

int main() {
  std::cout << "[part1] test solution 1: " << part1("input10_1.txt") << "\n";
  std::cout << "[part1] test solution 2: " << part1("input10_2.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input10_3.txt") << "\n";
  std::cout << "[part2] test solution 1: " << part2("input10_1.txt") << "\n";
  std::cout << "[part2] test solution 2: " << part2("input10_2.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input10_3.txt")  << "\n";
  return 0;
}