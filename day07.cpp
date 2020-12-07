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
  return tie(basket_name, bag_contents_vector);
}

int part1(const std::string &input) {
  auto lines = aoc_util::getLines(input);
  bag_map_t bag_map;
  
  for (const auto &line : lines) {
    auto [basket_name, bag_contents_vector] = bag_parser(line);
    // No other Bags
    if (bag_contents_vector.size() % 4 != 0) {
      continue;
    }

    for (int i = 0; i < bag_contents_vector.size(); i += 4) {
      auto contained_bag =
          std::accumulate(bag_contents_vector.begin() + i + 1,
                          bag_contents_vector.begin() + i + 4, std::string{});
      if (*(contained_bag.end() - 1) == 's') {
        contained_bag.pop_back();
      }

      bag_map.emplace(contained_bag, std::unordered_map<std::string, int>{});
      bag_map[contained_bag].emplace(basket_name, 0);
      bag_map[contained_bag][basket_name] += std::stoi(bag_contents_vector[i]);
    }
  }

  std::unordered_set<std::string> result_bags;
  contains(result_bags, bag_map, "shinygoldbag");
  return result_bags.size();
}

int main() {
  std::cout << "[part1] test solution : " << part1("input07_1.txt") << "\n";
  std::cout << "[part1] solution : " << part1("input07_2.txt") << "\n";
  return 0;
}