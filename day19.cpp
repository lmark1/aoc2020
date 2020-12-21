#include "util.hpp"
#include <boost/algorithm/string.hpp>
#include <numeric>
#include <unordered_map>

struct rule {
  char val = '?';
  bool has_value = false;
  std::vector<int> rule_set_1;
  std::vector<int> rule_set_2;

  friend std::ostream &operator<<(std::ostream &o, const rule &r) {
    o << "Val: " << r.val << " [";
    for (const auto &el : r.rule_set_1) {
      o << el << " ";
    }
    o << "] | [";
    for (const auto &el : r.rule_set_2) {
      o << el << " ";
    }
    o << "]\n";
    return o;
  }
};

using rule_map_t = std::unordered_map<int, rule>;

std::tuple<rule_map_t, std::vector<std::string>>
parse_input(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  rule_map_t rule_map;
  auto first_block_it =
      std::find_if(lines.begin(), lines.end(), [&](const auto &line) {
        if (line.empty()) {
          return true;
        }
        std::vector<std::string> result;
        boost::split(result, line, boost::is_any_of((" ")));
        int index = std::stoi(result.at(0).substr(0, result.at(0).size() - 1));

        rule r;
        if (result.size() == 2 && result.at(1).find('"') != std::string::npos) {
          r.val = result.at(1).at(1);
          r.has_value = true;
          rule_map.emplace(index, r);
          return false;
        }
        auto last = std::find(result.begin(), result.end(), "|");
        std::transform(result.begin() + 1, last,
                       std::back_inserter(r.rule_set_1), aoc_util::get_int);
        if (last != result.end()) {
          std::transform(last + 1, result.end(),
                         std::back_inserter(r.rule_set_2), aoc_util::get_int);
        }
        rule_map.emplace(index, r);
        return false;
      });
  return {rule_map, std::vector<std::string>(first_block_it + 1, lines.end())};
}

bool check_message(const std::vector<int> &rules, const rule_map_t &rule_map) {

  // std::cout << "Curr rules: ";
  // aoc_util::pprint(rules);
  // std::cout << "\n";
  // std::string s;
  //     getline(std::cin, s);
  if (rules.size() < rule_map.at(0).rule_set_1.size()) {
    return false;
  }

  if (rules == rule_map.at(0).rule_set_1) {
    return true;
  }

  for (const auto &other_rule : rule_map) {
    if (other_rule.first == 0 || other_rule.second.has_value) {
      continue;
    }

    // std::cout << other_rule.first << " " << other_rule.second;
    

    auto rule_set_1 = other_rule.second.rule_set_1;
    if (!rule_set_1.empty()) {
      std::vector<int> new_rules(rules.begin(), rules.end());
      auto start_of_ruleset = std::search(new_rules.begin(), new_rules.end(),
                                          rule_set_1.begin(), rule_set_1.end());

      if (start_of_ruleset != new_rules.end()) {
        new_rules.erase(start_of_ruleset, start_of_ruleset + rule_set_1.size());
        new_rules.insert(start_of_ruleset, other_rule.first);
        // std::cout << "Going in recursion with rules: ";
        // aoc_util::pprint(new_rules);
        // std::cout << "\n";
        if (check_message(new_rules, rule_map)) {
          return true;
        }
      }
    }

    auto rule_set_2 = other_rule.second.rule_set_2;
    if (!rule_set_2.empty()) {
      std::vector<int> new_rules(rules.begin(), rules.end());
      auto start_of_ruleset = std::search(new_rules.begin(), new_rules.end(),
                                          rule_set_2.begin(), rule_set_2.end());

      if (start_of_ruleset != new_rules.end()) {
        new_rules.erase(start_of_ruleset, start_of_ruleset + rule_set_2.size());
        new_rules.insert(start_of_ruleset, other_rule.first);
        // std::cout << "Going in recursion with rules: ";
        // aoc_util::pprint(new_rules);
        // std::cout << "\n";
        if (check_message(new_rules, rule_map)) {
          return true;
        }
      }
    }
  }

  return false;
}

std::tuple<int, int> get_ab_keys(const rule_map_t &rule_map) {
  int a_key = -1;
  int b_key = -1;
  for (const auto &it : rule_map) {
    if (!it.second.has_value) {
      continue;
    }

    if (it.second.val == 'a') {
      a_key = it.first;
    } else {
      b_key = it.first;
    }
  }
  return {a_key, b_key};
}

std::vector<int> convert_to_keys(const std::string &in, int a_val, int b_val) {
  std::vector<int> res;
  for (const auto &el : in) {
    if (el == 'a') {
      res.push_back(a_val);
      continue;
    }

    if (el == 'b') {
      res.push_back(b_val);
    }
  }
  return res;
}

int part1(const std::string &input) {
  auto [rule_map, messages] = parse_input(input);
  // std::cout << "\n";
  // for (const auto &it : rule_map) {
  //   std::cout << it.first << " " << it.second;
  // }

  std::cout << "Input: \n";
  int valid_counter = 0;
  auto [a_key, b_key] = get_ab_keys(rule_map);
  for (const auto &message : messages) {
    std::cout << message << " ";
    auto in = convert_to_keys(message, a_key, b_key);
    if (check_message(in, rule_map)) {
      valid_counter++;
      std::cout << " - passed";
    }
    std::cout << "\n";
  }
  return valid_counter;
}

// clang-format off
int main() {
  //std::cout << "[part1] test solution: " << part1("input/input19_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input/input19_2.txt") << "\n";
  return 0;
}