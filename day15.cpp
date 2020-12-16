#include "util.hpp"
#include <boost/algorithm/string.hpp>
#include <memory_resource>
#include <unordered_map>

using ll_t = long long;

std::vector<ll_t> parse_input(const std::string &input) {
  auto line = aoc_util::get_lines(input).front();
  std::cout << line << " --- ";
  std::vector<std::string> numbers_str;
  boost::split(numbers_str, line, boost::is_any_of(","));
  return aoc_util::get_numbers<ll_t>(numbers_str, aoc_util::get_llong);
}

struct number_info {
  ll_t spoken_count = 0LL;
  ll_t last_spoken_turn = -1LL;
  ll_t turn_before_last_spoken = -1LL;

  friend std::ostream &operator<<(std::ostream &o, const number_info &info) {
    o << "\tSpoken#: " << info.spoken_count
      << "\n\tLast turn: " << info.turn_before_last_spoken << "\n";
    return o;
  }
};

ll_t part1(const std::string &input, ll_t max_turn) {
  std::byte buffer[1000000];
  std::pmr::monotonic_buffer_resource rsrc(buffer, sizeof buffer);
  std::pmr::unordered_map<ll_t, number_info> number_info_map(&rsrc);
  auto numbers = parse_input(input);
  ll_t turn = 1LL;

  for (const auto &number : numbers) {
    number_info_map.emplace(number, number_info{1LL, turn, turn++});
  }

  auto last_number = numbers.back();
  static constexpr ll_t ZERO = 0LL;

  const auto add_number_at_turn = [&](const ll_t &number, const ll_t &turn) {
    number_info_map.emplace(number, number_info{});
    auto &info = number_info_map[number];
    info.spoken_count++;
    info.turn_before_last_spoken = info.last_spoken_turn;
    info.last_spoken_turn = turn;
    last_number = number;
  };

  while (turn < max_turn + 1LL) {
    auto last_number_info = number_info_map[last_number];

    // Number spoken only once
    if (last_number_info.spoken_count == 1) {
      add_number_at_turn(ZERO, turn++);
      continue;
    }

    last_number = (turn - 1LL) - last_number_info.turn_before_last_spoken;
    add_number_at_turn(last_number, turn++);
  }

  return last_number;
}

// clang-format off
int main() {
  std::cout << "[part1] test solution 1: " << part1("input/input15_1.txt", 2020) << "\n";
  std::cout << "[part1] test solution 2: " << part1("input/input15_2.txt", 2020) << "\n";
  std::cout << "[part1] test solution 3: " << part1("input/input15_3.txt", 2020) << "\n";
  std::cout << "[part1] test solution 4: " << part1("input/input15_4.txt", 2020) << "\n";
  std::cout << "[part1] test solution 5: " << part1("input/input15_5.txt", 2020) << "\n";
  std::cout << "[part1] test solution 6: " << part1("input/input15_6.txt", 2020) << "\n";
  std::cout << "[part1] test solution 7: " << part1("input/input15_7.txt", 2020) << "\n";
  std::cout << "[part1] solution: " << part1("input/input15_8.txt", 2020) << "\n\n";

  std::cout << "[part2] test solution 1: " << part1("input/input15_1.txt", 30000000) << "\n";
  std::cout << "[part2] test solution 2: " << part1("input/input15_2.txt", 30000000) << "\n";
  std::cout << "[part2] test solution 3: " << part1("input/input15_3.txt", 30000000) << "\n";
  std::cout << "[part2] test solution 4: " << part1("input/input15_4.txt", 30000000) << "\n";
  std::cout << "[part2] test solution 5: " << part1("input/input15_5.txt", 30000000) << "\n";
  std::cout << "[part2] test solution 6: " << part1("input/input15_6.txt", 30000000) << "\n";
  std::cout << "[part2] test solution 7: " << part1("input/input15_7.txt", 30000000) << "\n";
  std::cout << "[part2] solution: " << part1("input/input15_8.txt", 30000000) << "\n";
  return 0;
}