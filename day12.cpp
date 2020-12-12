#include "util.hpp"
#include <cmath>
#include <functional>
#include <unordered_map>

struct ship {
  enum heading_t { EAST = 'E', WEST = 'W', NORTH = 'N', SOUTH = 'S' };

  heading_t curr_heading = EAST;
  std::array<int, 2> position{0, 0};
  std::array<int, 2> waypoint{10, 1};
  std::unordered_map<char, std::function<void(int)>> function_map;
  std::unordered_map<char, std::function<void(int)>> waypoint_fmap;

  heading_t turn_heading(bool positive = true) {
    switch (curr_heading) {
    case EAST:
      if (positive) {
        return NORTH;
      } else {
        return SOUTH;
      }
      break;

    case NORTH:
      if (positive) {
        return WEST;
      } else {
        return EAST;
      }
      break;

    case WEST:
      if (positive) {
        return SOUTH;
      } else {
        return NORTH;
      }
      break;

    case SOUTH:
      if (positive) {
        return EAST;
      } else {
        return WEST;
      }
      break;

    default:
      throw std::runtime_error("wrong");
      break;
    }
  }

  ship() {
    function_map.emplace('E',
                         [&](const auto &command) { position[0] += command; });
    function_map.emplace('W',
                         [&](const auto &command) { position[0] -= command; });
    function_map.emplace('N',
                         [&](const auto &commnad) { position[1] += commnad; });
    function_map.emplace('S',
                         [&](const auto &commnad) { position[1] -= commnad; });
    function_map.emplace('F', [&](const auto &command) {
      switch (curr_heading) {

      case EAST:
        function_map['E'](command);
        break;

      case WEST:
        function_map['W'](command);
        break;

      case NORTH:
        function_map['N'](command);
        break;

      case SOUTH:
        function_map['S'](command);
        break;

      default:
        throw new std::runtime_error("nemoze");
        break;
      }
    });
    function_map.emplace('L', [&](const auto &command) {
      auto turn_rate = command / 90;
      while (turn_rate > 0) {
        curr_heading = turn_heading();
        turn_rate--;
      }
    });
    function_map.emplace('R', [&](const auto &command) {
      auto turn_rate = command / 90;
      while (turn_rate > 0) {
        curr_heading = turn_heading(false);
        turn_rate--;
      }
    });

    ////////////////////////////////////////////////////////

    waypoint_fmap.emplace('E',
                          [&](const auto &command) { waypoint[0] += command; });
    waypoint_fmap.emplace('W',
                          [&](const auto &command) { waypoint[0] -= command; });
    waypoint_fmap.emplace('N',
                          [&](const auto &commnad) { waypoint[1] += commnad; });
    waypoint_fmap.emplace('S',
                          [&](const auto &commnad) { waypoint[1] -= commnad; });
    waypoint_fmap.emplace('F', [&](const auto &command) {
      position[0] += command * waypoint[0];
      position[1] += command * waypoint[1];
    });
    waypoint_fmap.emplace('R', [&](const auto &command) {
      auto copy_waypoint = waypoint;
      auto rad_c = command * M_PI / 180.0;
      int cos_command = cos(rad_c);
      int sin_command = sin(rad_c);
      waypoint[0] =
          cos_command * copy_waypoint[0] + sin_command * copy_waypoint[1];
      waypoint[1] =
          -sin_command * copy_waypoint[0] + cos_command * copy_waypoint[1];
    });
    waypoint_fmap.emplace('L', [&](const auto &command) {
      auto copy_waypoint = waypoint;
      auto rad_c = - command * M_PI / 180.0;
      int cos_command = cos(rad_c);
      int sin_command = sin(rad_c);
      waypoint[0] =
          cos_command * copy_waypoint[0] + sin_command * copy_waypoint[1];
      waypoint[1] =
          -sin_command * copy_waypoint[0] + cos_command * copy_waypoint[1];
    });
  }

  void apply_command(const std::string &command) {
    auto c = command.at(0);
    auto val = std::stoi(command.substr(1, command.size() - 1));
    function_map[c](val);
  }

  void apply_waypoint_command(const std::string &command) {
    auto c = command.at(0);
    auto val = std::stoi(command.substr(1, command.size() - 1));
    waypoint_fmap[c](val);
    // std::cout << c << " " << val << "\n";
    // std::cout << "Ship [" << position[0] << ", " << position[1] << "]\n";
    // std::cout << "Waypoint [" << waypoint[0] << ", " << waypoint[1] << "]\n\n";
    // std::string b;
    // getline(std::cin, b);
  }
};

int part1(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  ship s;
  for (const auto &line : lines) {
    s.apply_command(line);
  }
  return abs(s.position[0]) + abs(s.position[1]);
}

int part2(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  ship s;
  for (const auto &line : lines) {
    s.apply_waypoint_command(line);
  }
  return abs(s.position[0]) + abs(s.position[1]);
}

int main() {
  std::cout << "[part1] test solution: " << part1("input12_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input12_2.txt") << "\n";
  std::cout << "[part2] test solution: " << part2("input12_1.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input12_2.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input12_3.txt") << "\n";
  return 0;
}