#include "util.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

using bag_map_t =
    std::unordered_map<std::string, std::unordered_map<std::string, int>>;
void contains(std::unordered_set<std::string> &bag_set, bag_map_t &bag_map,
              const std::string &bag) {
  for (auto &other_bags : bag_map[bag]) {
    bag_set.emplace(other_bags.first);
    contains(bag_set, bag_map, other_bags.first);
  }
}

int count_bags(bag_map_t &bag_map, const std::string &bag) {
  int bag_count = 0;

  for (const auto &item : bag_map[bag]) {
    bag_count += item.second;
    bag_count += item.second * count_bags(bag_map, item.first);
  }

  return bag_count;
}

using bag_tuples = std::tuple<std::string, std::vector<std::string>>;
bag_tuples bag_parser(const std::string &line) {
  std::vector<std::string> res;
  boost::split(res, line, boost::is_any_of(" .,"));

  // Find out Bag name
  std::vector<std::string> bag_name_vector;
  std::copy_n(res.begin(), 3, std::back_inserter(bag_name_vector));
  bag_name_vector[2].pop_back();
  auto basket_name = std::accumulate(bag_name_vector.begin(),
                                     bag_name_vector.end(), std::string{});

  // Find Out Bag Capacity
  std::vector<std::string> bag_contents_vector;
  std::copy_if(res.begin() + 4, res.end(),
               std::back_inserter(bag_contents_vector),
               [&](const auto &el) { return !el.empty(); });

  std::vector<std::string> compressed_bag_contents;
  if (bag_contents_vector.size() % 4 != 0) {
    return tie(basket_name, compressed_bag_contents);
  }

  for (int i = 0; i < bag_contents_vector.size(); i += 4) {
    compressed_bag_contents.push_back(bag_contents_vector.at(i));
    auto contained_bag =
        std::accumulate(bag_contents_vector.begin() + i + 1,
                        bag_contents_vector.begin() + i + 4, std::string{});
    if (*(contained_bag.end() - 1) == 's') {
      contained_bag.pop_back();
    }
    compressed_bag_contents.push_back(contained_bag);
  }

  return tie(basket_name, compressed_bag_contents);
}

int part1(const std::string &input) {
  auto lines = aoc_util::getLines(input);
  bag_map_t bag_map;

  for (const auto &line : lines) {
    auto [basket_name, bag_contents_vector] = bag_parser(line);
    // No other Bags
    if (bag_contents_vector.empty()) {
      continue;
    }

    for (int i = 0; i < bag_contents_vector.size(); i += 2) {
      auto contained_bag = bag_contents_vector.at(i + 1);
      bag_map.emplace(contained_bag, std::unordered_map<std::string, int>{});
      bag_map[contained_bag].emplace(basket_name, 0);
      bag_map[contained_bag][basket_name] += std::stoi(bag_contents_vector[i]);
    }
  }

  std::unordered_set<std::string> result_bags;
  contains(result_bags, bag_map, "shinygoldbag");
  return result_bags.size();
}

int part2(const std::string &input) {
  auto lines = aoc_util::getLines(input);
  bag_map_t bag_map;

  // Build bag map
  for (const auto &line : lines) {
    auto [basket_name, bag_contents_vector] = bag_parser(line);
    // No other Bags
    if (bag_contents_vector.empty()) {
      continue;
    }

    for (int i = 0; i < bag_contents_vector.size(); i += 2) {
      auto contained_bag = bag_contents_vector.at(i + 1);
      auto contained_amount = std::stoi(bag_contents_vector[i]);

      bag_map.emplace(basket_name, std::unordered_map<std::string, int>{});
      bag_map[basket_name].emplace(contained_bag, 0);
      bag_map[basket_name][contained_bag] += contained_amount;
    }
  }

  return count_bags(bag_map, "shinygoldbag");
}

int main() {
  std::cout << "[part1] test solution : " << part1("input07_1.txt") << "\n";
  std::cout << "[part1] solution : " << part1("input07_2.txt") << "\n";
  std::cout << "[part2] test1 solution : " << part2("input07_1.txt") << "\n";
  std::cout << "[part2] test2 solution : " << part2("input07_3.txt") << "\n";
  std::cout << "[part2] solution : " << part2("input07_2.txt") << "\n";
  return 0;
}