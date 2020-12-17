#include "util.hpp"
#include <cmath>
#include <numeric>
#include <unordered_map>

using pos_t = std::tuple<long long, long long, long long>;
struct pos_hash : public std::unary_function<pos_t, std::size_t> {
  std::size_t operator()(const pos_t &key) const {
    return std::get<0>(key) ^ std::get<1>(key) ^ std::get<2>(key);
  }
};
using galaxy_map_t = std::unordered_map<pos_t, char, pos_hash>;

galaxy_map_t get_galaxy_map(const std::string &input) {
  const auto lines = aoc_util::get_lines(input);
  const auto matrix = aoc_util::get_matrix(lines);
  const auto HEIGHT = lines.size();
  const auto WIDTH = lines.front().size();
  galaxy_map_t galaxy_map;
  for (auto i = 0; i < HEIGHT; i++) {
    for (auto j = 0; j < WIDTH; j++) {
      galaxy_map.emplace(std::make_tuple(i, j, 0), matrix[i][j]);
    }
  }
  return galaxy_map;
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

int active_neighbour_count(const galaxy_map_t &gmap, const pos_t &pos) {
  auto adjacent_positions = get_adjacent_positions(pos);
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
};

void update_bounds(bounds &b, const pos_t &pos) {
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
}

void pretty_print(const galaxy_map_t &gmap, const bounds &b) {
  using matrix3d = std::vector<aoc_util::matrix<char>>;
  matrix3d m(b.MAX_Z - b.MIN_Z + 1,
             aoc_util::matrix<char>(b.MAX_X - b.MIN_X + 1,
                                    std::vector<char>(b.MAX_Y - b.MIN_Y + 1, '.')));
  for (const auto &it : gmap) {
    auto x = std::get<0>(it.first);
    auto y = std::get<1>(it.first);
    auto z = std::get<2>(it.first);
    m[llabs(b.MIN_Z) + z][llabs(b.MIN_X) + x][llabs(b.MIN_Y) + y] = it.second;
  }

  for (auto k = 0; k < m.size(); k++) {
    std::cout << "Layer: " << k + b.MIN_Z << "\n";
    for (auto i = 0; i < m.front().size(); i++) {
      for (auto j = 0; j < m.front().front().size(); j++) {
        std::cout << m[k][i][j];
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }

  std::cout << "//////////////////////////////////////////////\n";
}

long long part1(const std::string &input) {
  auto galaxy_map = get_galaxy_map(input);

  int cycle_count = 0;
  while (cycle_count < 6) {
    galaxy_map_t new_map(galaxy_map.begin(), galaxy_map.end());

    bounds b;
    for (const auto &it : new_map) {

      update_bounds(b, it.first);
      if (it.second == '.') {
        continue;
      }
      auto nearby = get_adjacent_positions(it.first);
      for (const auto &pos : nearby) {
        galaxy_map.emplace(pos, '.');
        update_bounds(b, pos);
      }
    }
    new_map = galaxy_map;

    for (const auto &item : galaxy_map) {

      auto active_neighbours = active_neighbour_count(galaxy_map, item.first);
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

// clang-format off
int main() {
  std::cout << "[part1] test solution: " << part1("input/input17_1.txt") << "\n";
  std::cout << "[part1] test solution: " << part1("input/input17_2.txt") << "\n";
  return 0;
}