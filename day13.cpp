#include "util.hpp"
#include <boost/algorithm/string.hpp>

int part1(const std::string &input) {
  auto lines = aoc_util::get_lines(input);

  std::vector<std::string> bus_ids;
  boost::split(bus_ids, lines.at(1), boost::is_any_of(","));

  std::vector<int> valid_ids;
  std::for_each(bus_ids.begin(), bus_ids.end(), [&](const auto &el) {
    if (el == "x") {
      return;
    }
    valid_ids.emplace_back(std::stoi(el));
  });
  
  auto timestamp = std::stoi(lines.at(0));
  double smallest_distance = 1e9;
  int best_id = -1;
  for (const auto& id : valid_ids)
  {
    double div = ceil(static_cast<double>(timestamp) / id) * id;
    if (div < smallest_distance)
    {
      smallest_distance = div;
      best_id = id;
    }
  }

  return best_id * (smallest_distance - timestamp);
}

int main() {
  std::cout << "[part1] test solution: " << part1("input13_1.txt") << "\n";
  std::cout << "[part1] test solution: " << part1("input13_2.txt") << "\n";
  return 0;
}