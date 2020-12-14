#include "util.hpp"
#include <bitset>
#include <boost/algorithm/string.hpp>

struct Computer {
  std::string bitmask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
  std::vector<unsigned long long> memory;

  void apply_command(const std::string &command, const std::string &argument) {
    //std::cout << "Command: " << command << ", Arg: " << argument << "\n";

    if (command == "mask") {
      bitmask = argument;
      return;
    }

    if (command.rfind("mem", 0) == 0) {
      std::vector<std::string> cmd_vec;
      boost::split(cmd_vec, command, boost::is_any_of("[]"));
      std::size_t index = std::stoll(cmd_vec.at(1));
      if (memory.size() < index + 1) {
        memory.resize(index + 1);
      }
      memory[index] = apply_bitmask(argument);
      //std::cout << "mem[" << index << "] = " << memory[index] << "\n";
      return;
    }
  }

  unsigned long long apply_bitmask(const std::string &arg) {
    auto arg_bits = std::bitset<36>(std::stoll(arg));
    auto result_bits = std::bitset<36>(0);
    for (int i = 0; i < 36; i++) {
      auto curr_bitmask = bitmask.at(i);

      if (curr_bitmask == 'X') {
        result_bits.set(35 - i, static_cast<bool>(arg_bits[35 - i]));
        continue;
      }

      if (curr_bitmask == '1') {
        result_bits.set(35 - i, true);
        continue;
      }

      if (curr_bitmask == '0') {
        result_bits.set(35 - i, false);
        continue;
      }

    }

    return result_bits.to_ullong();
  }

  unsigned long long sum()
  {
    unsigned long long res = 0;
    for (const auto& ull : memory){
      res+=ull;
    }
    return res;
  }
};

unsigned long long part1(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  Computer c;
  for (const auto &line : lines) {
    std::vector<std::string> split;
    boost::split(split, line, boost::is_any_of(" "));
    c.apply_command(split.at(0), split.at(2));
  }
  return c.sum();
}

int main() {
  std::cout << "[part1] test solution: " << part1("input14_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input14_2.txt") << "\n";
  return 0;
}