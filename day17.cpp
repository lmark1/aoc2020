#include "util.hpp"
#include <cmath>
#include <numeric>
#include <type_traits>
#include <unordered_map>

using pos_t = std::tuple<long long, long long, long long>;
using hyper_pos_t = std::tuple<long long, long long, long long, long long>;

template <typename Key>
struct key_hash : public std::unary_function<Key, std::size_t> {
  std::size_t operator()(const Key &key) const {
    if constexpr (std::tuple_size<Key>() == 3) {
      return std::get<0>(key) ^ std::get<1>(key) ^ std::get<2>(key);
    } else if constexpr (std::tuple_size<Key>() == 4) {
      return std::get<0>(key) ^ std::get<1>(key) ^ std::get<2>(key) ^
             std::get<3>(key);
    }
  }
};

using galaxy_map_t = std::unordered_map<pos_t, char, key_hash<pos_t>>;
using hyper_map_t = std::unordered_map<hyper_pos_t, char, key_hash<hyper_pos_t>>;

template <typename Key>
using adj_map_t = std::unordered_map<Key, std::vector<Key>, key_hash<Key>>;

template <typename Map> Map get_map(const std::string &input) {
  const auto lines = aoc_util::get_lines(input);
  const auto matrix = aoc_util::get_matrix(lines);
  const auto HEIGHT = lines.size();
  const auto WIDTH = lines.front().size();
  Map map;
  for (std::size_t i = 0; i < HEIGHT; i++) {
    for (std::size_t j = 0; j < WIDTH; j++) {

      constexpr auto key_size = std::tuple_size<typename Map::key_type>();
      if constexpr (key_size == 3) {
        map.emplace(std::make_tuple(i, j, 0), matrix[i][j]);
      } else if constexpr (key_size == 4) {
        map.emplace(std::make_tuple(i, j, 0, 0), matrix[i][j]);
      }
    }
  }
  return map;
}

std::vector<pos_t> get_adjacent_positions(const pos_t &init_pos) {
  std::vector<pos_t> adjacent_pos;
  for (auto i = -1; i < 2; i++) {
    for (auto j = -1; j < 2; j++) {
      for (auto k = -1; k < 2; k++) {
        if (i == 0 && j == 0 && k == 0) {
          continue;
        }
        adjacent_pos.emplace_back(std::get<0>(init_pos) + i,
                                  std::get<1>(init_pos) + j,
                                  std::get<2>(init_pos) + k);
      }
    }
  }
  return adjacent_pos;
}

std::vector<hyper_pos_t>
get_adjacent_hyperpositions(const hyper_pos_t &init_pos) {
  std::vector<hyper_pos_t> adjacent_hyperpos;
  for (auto i = -1; i < 2; i++) {
    for (auto j = -1; j < 2; j++) {
      for (auto k = -1; k < 2; k++) {
        for (auto l = -1; l < 2; l++) {
          if (i == 0 && j == 0 && k == 0 && l == 0) {
            continue;
          }
          adjacent_hyperpos.emplace_back(
              std::get<0>(init_pos) + i, std::get<1>(init_pos) + j,
              std::get<2>(init_pos) + k, std::get<3>(init_pos) + l);
        }
      }
    }
  }
  return adjacent_hyperpos;
}

template <typename Tuple>
std::vector<Tuple> get_adjacent(const Tuple &init_pos) {
  if constexpr (std::tuple_size<Tuple>::value == 3) {
    return get_adjacent_positions(init_pos);
  } else if constexpr (std::tuple_size<Tuple>::value == 4) {
    return get_adjacent_hyperpositions(init_pos);
  }
}

template <typename Map, typename Key>
int active_neighbour_count(const Map &gmap,
                           const std::vector<Key> &adjacent_positions) {
  int active_neighbours = 0;
  for (const auto &adj_pos : adjacent_positions) {
    if (!gmap.contains(adj_pos)) {
      continue;
    }

    if (gmap.at(adj_pos) == '#') {
      active_neighbours++;
    }
  }
  return active_neighbours;
}

template <typename Map> long long solution(const std::string &input) {
  auto galaxy_map = get_map<Map>(input);
  adj_map_t<typename Map::key_type> adj_map;

  const auto get_adjacent_cached = [&](const auto &key) {
    if (!adj_map.contains(key)) {
      auto nearby = get_adjacent(key);
      adj_map.emplace(key, nearby);
    }
    return adj_map[key];
  };

  int cycle_count = 0;
  while (cycle_count < 6) {

      std::cout << "Count: " << cycle_count << "\n";

    Map new_map(galaxy_map.begin(), galaxy_map.end());
    for (const auto &it : new_map) {
      if (it.second == '.') {
        continue;
      }

      auto nearby = get_adjacent_cached(it.first);
      for (const auto &pos : nearby) {
        galaxy_map.emplace(pos, '.');
      }
    }
    new_map = galaxy_map;

    for (const auto &item : galaxy_map) {
      auto adjacent_positions = get_adjacent_cached(item.first);
      auto active_neighbours =
          active_neighbour_count(galaxy_map, adjacent_positions);

      auto curr_cube = item.second;
      if (curr_cube == '#' &&
          !(active_neighbours == 2 || active_neighbours == 3)) {
        new_map[item.first] = '.';
        continue;
      }

      if (curr_cube == '.' && active_neighbours == 3) {
        new_map[item.first] = '#';
        continue;
      }
    }

    galaxy_map = std::move(new_map);
    cycle_count++;
  }

  return std::accumulate(galaxy_map.begin(), galaxy_map.end(), 0LL,
                         [&](long long sum, const auto &item) {
                           long long add = item.second == '#' ? 1LL : 0LL;
                           return sum + add;
                         });
}

long long part1(const std::string &input) {
  return solution<galaxy_map_t>(input);
}

long long part2(const std::string &input) {
  return solution<hyper_map_t>(input);
}

// clang-format off
int main() {
  std::cout << "[part1] test solution: " << part1("input/input17_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input/input17_2.txt") << "\n";
  std::cout << "[part2] test solution: " << part2("input/input17_1.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input/input17_2.txt") << "\n";
  return 0;
}