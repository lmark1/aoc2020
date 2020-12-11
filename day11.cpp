#include "util.hpp"
#include <thread>

template <typename T> using index_pair = std::pair<T, T>;

template <typename T>
std::vector<std::pair<T, T>>
get_nearest_indices(const std::pair<T, T> &indices,
                    const aoc_util::matrix<char> &orig_matrix) {

  const auto HEIGHT = orig_matrix.size();
  const auto WIDTH = orig_matrix.front().size();
  std::vector<std::pair<T, T>> pairs;
  auto curr_elem = orig_matrix.at(indices.first).at(indices.second);
  for (auto dx = -1; dx < 2; dx++) {
    for (auto dy = -1; dy < 2; dy++) {

      if (dx == 0 && dy == 0) {
        continue;
      }

      auto i = indices.first + dx;
      auto j = indices.second + dy;

      while (!(i < 0 || j < 0 || i + 1 > HEIGHT || j + 1 > WIDTH)) {
        auto nearest_elem = orig_matrix.at(i).at(j);
        if (nearest_elem != '.') {
          pairs.emplace_back(std::make_pair(i, j));
          break;
        }
        i += dx;
        j += dy;
      }
    }
  }

  return pairs;
}

template <typename T>
std::vector<index_pair<T>> get_adjacent_indices(const std::pair<T, T> &indices,
                                                const int WIDTH,
                                                const int HEIGHT) {
  std::vector<index_pair<T>> valid_indices;

  for (auto dx = -1; dx < 2; dx++) {
    for (auto dy = -1; dy < 2; dy++) {

      if (dx == 0 && dy == 0) {
        continue;
      }

      auto i = indices.first + dx;
      auto j = indices.second + dy;

      if (i < 0 || j < 0 || i + 1 > HEIGHT || j + 1 > WIDTH) {
        continue;
      }

      // std::cout << "( " << i << ", " << j << " )\n";
      valid_indices.emplace_back(std::make_pair(i, j));
    }
  }
  return valid_indices;
}

template <typename T>
bool apply_rules(const aoc_util::matrix<char> &m,
                 const std::vector<index_pair<T>> &indices, char &curr_el,
                 const int treshold = 3) {

  int occupied_count = 0;
  for (const auto &index : indices) {
    auto el = m[index.first][index.second];
    if (el == '#') {
      occupied_count++;
    }
  }

  if (curr_el == 'L' && occupied_count == 0) {
    curr_el = '#';
    return true;
  }

  if (curr_el == '#' && occupied_count > treshold) {
    curr_el = 'L';
    return true;
  }

  return false;
}

template <typename F>
int generalized_solution(const std::string &input, F &&f) {
  auto lines = aoc_util::get_lines(input);
  auto matrix = aoc_util::get_matrix(lines);

  const auto HEIGHT = matrix.size();
  const auto WIDTH = matrix.front().size();

  bool seat_changed = true;
  int iteration_count = 0;

  while (seat_changed) {
    auto orig_matrix = matrix;
    seat_changed = false;
    for (auto i = 0; i < HEIGHT; i++) {
      for (auto j = 0; j < WIDTH; j++) {
        auto &el = matrix[i][j];
        seat_changed |= f(el, std::make_pair(i, j), orig_matrix);
      }
    }
  }

  int occupied_count = 0;
  for (const auto &row : matrix) {
    for (const auto &el : row) {
      if (el == '#') {
        occupied_count++;
      }
    }
  }
  return occupied_count;
}

int part1(const std::string &input) {
  return generalized_solution(
      input, [&](auto &el, auto index_pair, const auto &m) {
        auto adjacent_indices =
            get_adjacent_indices(index_pair, m.front().size(), m.size());
        return apply_rules(m, adjacent_indices, el);
      });
}

int part2(const std::string &input) {
  return generalized_solution(input, [&](auto &el, auto index_pair, auto &m) {
    auto adjacent_indices = get_nearest_indices(index_pair, m);
    return apply_rules(m, adjacent_indices, el, 4);
  });
}

int main() {
  std::cout << "[part1] test solution: " << part1("input11_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input11_2.txt") << "\n";
  std::cout << "[part2] test solution: " << part2("input11_1.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input11_2.txt") << "\n";
  return 0;
}