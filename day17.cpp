#include "util.hpp"
#include <cmath>
#include <numeric>
#include <unordered_map>

using pos_t = std::tuple<long long, long long, long long>;
using hyper_pos_t = std::tuple<long long, long long, long long, long long>;

struct pos_hash : public std::unary_function<pos_t, std::size_t> {
  std::size_t operator()(const pos_t &key) const {
    return std::get<0>(key) ^ std::get<1>(key) ^ std::get<2>(key);
  }
};
struct hyper_pos_hash : public std::unary_function<hyper_pos_t, std::size_t> {
  std::size_t operator()(const hyper_pos_t &key) const {
    return std::get<0>(key) ^ std::get<1>(key) ^ std::get<2>(key) ^
           std::get<3>(key);
  }
};
using galaxy_map_t = std::unordered_map<pos_t, char, pos_hash>;
using hyper_map_t = std::unordered_map<hyper_pos_t, char, hyper_pos_hash>;

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

struct bounds {
  long long MIN_X = 1e9;
  long long MAX_X = -1e9;
  long long MIN_Y = 1e9;
  long long MAX_Y = -1e9;
  long long MIN_Z = 1e9;
  long long MAX_Z = -1e9;
  long long MIN_W = 1e9;
  long long MAX_W = -1e9;
};

template <typename Tuple> void update_bounds(bounds &b, const Tuple &pos) {
  auto x = std::get<0>(pos);
  auto y = std::get<1>(pos);
  auto z = std::get<2>(pos);

  if (x < b.MIN_X) {
    b.MIN_X = x;
  } else if (x > b.MAX_X) {
    b.MAX_X = x;
  }

  if (y < b.MIN_Y) {
    b.MIN_Y = y;
  } else if (y > b.MAX_Y) {
    b.MAX_Y = y;
  }

  if (z < b.MIN_Z) {
    b.MIN_Z = z;
  } else if (z > b.MAX_Z) {
    b.MAX_Z = z;
  }

  if constexpr (std::tuple_size<Tuple>::value > 3) {
    auto w = std::get<3>(pos);
    if (w < b.MIN_W) {
      b.MIN_W = w;
    } else if (w > b.MAX_W) {
      b.MAX_W = w;
    }
  }
}

template <typename Map> long long solution(const std::string &input) {
  auto galaxy_map = get_map<Map>(input);

  int cycle_count = 0;
  while (cycle_count < 6) {
    Map new_map(galaxy_map.begin(), galaxy_map.end());
    bounds b;
    for (const auto &it : new_map) {

      update_bounds(b, it.first);
      if (it.second == '.') {
        continue;
      }
      auto nearby = get_adjacent(it.first);
      for (const auto &pos : nearby) {
        galaxy_map.emplace(pos, '.');
        update_bounds(b, pos);
      }
    }
    new_map = galaxy_map;

    for (const auto &item : galaxy_map) {
      auto adjacent_positions = get_adjacent(item.first);
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