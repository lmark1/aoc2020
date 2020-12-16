#include "util.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <numeric>
#include <unordered_map>

struct range {
  int lower_1;
  int upper_1;
  int lower_2;
  int upper_2;

  range(int l1, int u1, int l2, int u2)
      : lower_1(l1), upper_1(u1), lower_2(l2), upper_2(u2) {}
};

using field_map_t = std::unordered_map<std::string, range>;

std::tuple<field_map_t, std::vector<int>, aoc_util::matrix<int>>
parse_input(const std::vector<std::string> &lines) {

  field_map_t field_map;
  auto block_1 = std::find_if(lines.begin(), lines.end(), [&](const auto &el) {
    if (el.empty()) {
      return true;
    }

    std::vector<std::string> split_result;
    boost::split(split_result, el, boost::is_any_of(" :-"));
    auto last_index = split_result.size() - 1;
    auto name =
        last_index > 6 ? split_result[0] + split_result[1] : split_result[0];
    field_map.try_emplace(name, std::stoi(split_result[last_index - 4]),
                          std::stoi(split_result[last_index - 3]),
                          std::stoi(split_result[last_index - 1]),
                          std::stoi(split_result[last_index]));
    return false;
  });

  const auto get_ticket = [](const auto &in) {
    std::vector<std::string> split_result;
    boost::split(split_result, in, boost::is_any_of(","));
    return aoc_util::get_numbers<int>(split_result, aoc_util::get_int);
  };

  std::vector<int> my_ticket;
  auto block_2 = std::find_if(block_1 + 1, lines.end(), [&](const auto &el) {
    if (el.empty()) {
      return true;
    }

    if (el == "your ticket:") {
      return false;
    }

    my_ticket = get_ticket(el);
    return false;
  });

  aoc_util::matrix<int> other_tickets;
  std::for_each(block_2 + 1, lines.end(), [&](const auto &el) {
    if (el == "nearby tickets:") {
      return;
    }

    other_tickets.emplace_back(get_ticket(el));
  });
  return {field_map, my_ticket, other_tickets};
}

long long verify_number(int ticket_number, const field_map_t &field_map) {
  for (const auto &field : field_map) {
    auto range = field.second;
    if ((ticket_number >= range.lower_1 && ticket_number <= range.upper_1) ||
        (ticket_number >= range.lower_2 && ticket_number <= range.upper_2)) {
      return 0L;
    }
  }
  return static_cast<long long>(ticket_number);
}

long long verify_ticket(const field_map_t &field_map,
                        const std::vector<int> &ticket) {
  long long sum = 0LL;
  for (const auto &ticket_number : ticket) {
    sum += verify_number(ticket_number, field_map);
  }
  return sum;
}

long long part1(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  auto [field_map, my_ticket, other_tickets] = parse_input(lines);
  return std::accumulate(other_tickets.begin(), other_tickets.end(), 0LL,
                         [&](long long sum, const auto &ticket) {
                           return sum + verify_ticket(field_map, ticket);
                         });
}

long long part2(const std::string& input) {
  auto lines = aoc_util::get_lines(input);
  auto [field_map, my_ticket, other_tickets] = parse_input(lines);

  // Remove invalid elements
  aoc_util::matrix<int> valid_tickets;
  std::copy_if(other_tickets.begin(), other_tickets.end(), std::back_inserter(valid_tickets), [&](const auto& el) {
    return verify_ticket(field_map, el) == 0LL;
  });


  return 0LL;
}

// clang-format off
int main() {
  std::cout << "[part1] test solution: " << part1("input/input16_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input/input16_2.txt") << "\n";
  return 0;
}