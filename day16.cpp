#include "util.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <numeric>
#include <set>
#include <unordered_map>

struct range {
  int lower_1;
  int upper_1;
  int lower_2;
  int upper_2;
  std::set<int> valid_idxs;

  range(int l1, int u1, int l2, int u2)
      : lower_1(l1), upper_1(u1), lower_2(l2), upper_2(u2) {}
};

struct verification {
  long long number;
  bool valid;
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

bool is_in_range(int el, const range &range) {
  return (el >= range.lower_1 && el <= range.upper_1) ||
         (el >= range.lower_2 && el <= range.upper_2);
}

verification verify_number(int ticket_number, const field_map_t &field_map) {
  for (const auto &field : field_map) {
    auto range = field.second;
    if (is_in_range(ticket_number, range)) {
      return {0L, true};
    }
  }
  return {static_cast<long long>(ticket_number), false};
}

verification verify_ticket(const field_map_t &field_map,
                           const std::vector<int> &ticket) {
  long long sum = 0LL;
  bool check = true;
  for (const auto &ticket_number : ticket) {
    auto res = verify_number(ticket_number, field_map);
    check &= res.valid;
    sum += res.number;
  }
  return {sum, check};
}

void find_corresponding_field(const std::vector<int> &col,
                              field_map_t &field_map, std::size_t col_idx) {
  for (auto &field : field_map) {
    auto &range = field.second;
    auto check = std::all_of(col.begin(), col.end(), [&](const auto &el) {
      return is_in_range(el, range);
    });

    if (check) {
      range.valid_idxs.insert(col_idx);
    }
  }
}

void filter_valid_indices(field_map_t &field_map, int index_count) {
  std::set<int> singles;
  int singles_count = 0;
  auto in_singles = [&](const auto &el) {
    return std::binary_search(singles.begin(), singles.end(), el);
  };
  while (singles_count < index_count) {

    singles_count = 0;
    for (auto &field : field_map) {
      auto &valid_indices = field.second.valid_idxs;

      // if there is more do a set difference and save it
      if (valid_indices.size() > 1) {
        std::set<int> diff;
        std::set_difference(valid_indices.begin(), valid_indices.end(),
                            singles.begin(), singles.end(),
                            std::inserter(diff, diff.begin()));
        valid_indices.swap(diff);
      }

      // If there is only 1 add it to singles
      if (valid_indices.size() == 1) {
        singles.insert(*valid_indices.begin());
        singles_count++;
      }
    }
  }
}

long long part1(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  auto [field_map, my_ticket, other_tickets] = parse_input(lines);
  return std::accumulate(
      other_tickets.begin(), other_tickets.end(), 0LL,
      [&, field_map = std::ref(field_map)](long long sum, const auto &ticket) {
        return sum + verify_ticket(field_map.get(), ticket).number;
      });
}

long long part2(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  auto [field_map, my_ticket, other_tickets] = parse_input(lines);

  // Remove invalid elements
  aoc_util::matrix<int> valid_tickets;
  std::copy_if(other_tickets.begin(), other_tickets.end(),
               std::back_inserter(valid_tickets),
               [&, field_map = std::ref(field_map)](const auto &el) {
                 return verify_ticket(field_map.get(), el).valid;
               });

  const auto HEIGHT = valid_tickets.size();
  const auto WIDTH = valid_tickets.front().size();
  for (std::size_t i = 0; i < WIDTH; i++) {
    std::vector<int> column_vals;
    for (std::size_t j = 0; j < HEIGHT; j++) {
      column_vals.push_back(valid_tickets[j][i]);
    }
    find_corresponding_field(column_vals, field_map, i);
  }

  filter_valid_indices(field_map, WIDTH);
  return std::accumulate(
      field_map.begin(), field_map.end(), 1LL,
      [&, my_ticket = std::ref(my_ticket)](long long mul, const auto &field) {
        if (field.first.rfind("departure", 0) != 0) {
          return mul;
        }
        int idx = *field.second.valid_idxs.begin();
        return mul * my_ticket.get().at(idx);
      });
}

// clang-format off
int main() {
  std::cout << "[part1] test solution: " << part1("input/input16_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input/input16_2.txt") << "\n";
  std::cout << "[part2] test solution: " << part2("input/input16_3.txt") << "\n"; 
  std::cout << "[part2] test solution: " << part2("input/input16_2.txt") << "\n"; 
  return 0;
}