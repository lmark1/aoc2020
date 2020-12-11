#include "util.hpp"
#include <thread>

template <typename T> using index_pair = std::pair<T, T>;

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
                 const std::vector<index_pair<T>> &indices, char &curr_el) {

  int occupied_count = 0;
  // std::cout << "\n";
  for (const auto &index : indices) {
    auto el = m[index.first][index.second];
    // std::cout << el << " ";
    if (el == '#') {
      occupied_count++;
    }
  }
  // std::cout << "\n" << occupied_count << "\n";

  if (curr_el == 'L' && occupied_count == 0) {
    curr_el = '#';
    return true;
  }

  if (curr_el == '#' && occupied_count > 3) {
    // std::cout << "HERE\n";
    curr_el = 'L';
    return true;
  }

  return false;
}

int part1(const std::string &input, bool visualize = false) {
  auto lines = aoc_util::get_lines(input);
  auto matrix = aoc_util::get_matrix(lines);

  const auto HEIGHT = matrix.size();
  const auto WIDTH = matrix.front().size();

  bool seat_changed = true;
  int iteration_count = 0;

  while (seat_changed) {
    auto orig_matrix = matrix;
    if (visualize) {
      aoc_util::sleep(std::chrono::milliseconds(100));
      aoc_util::ClearScreen();
    }
    seat_changed = false;
    for (auto i = 0; i < HEIGHT; i++) {
      for (auto j = 0; j < WIDTH; j++) {
        auto &el = matrix[i][j];
        if (visualize) {
          std::cout << el << " ";
        }
        auto adjacent_indices =
            get_adjacent_indices(std::make_pair(i, j), WIDTH, HEIGHT);
        seat_changed |= apply_rules(orig_matrix, adjacent_indices, el);
      }
      if (visualize) {
        std::cout << "\n";
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

int part2(const std::string &input) { return 0; }

int main() {
  auto test_p1 = part1("input11_1.txt");
  std::cout << "[part1] test solution: " << test_p1 << "\n";
  auto p1 = part1("input11_2.txt");
  std::cout << "[part1] solution: " << p1 << "\n";
  return 0;
}