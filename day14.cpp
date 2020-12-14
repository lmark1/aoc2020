#include "util.hpp"
#include <bitset>
#include <boost/algorithm/string.hpp>

struct Computer {
  std::string bitmask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
  std::vector<unsigned long long> memory;
  std::vector<std::string> bitmasks;

  template <bool adress_switching>
  void apply_command(const std::string &command, const std::string &argument) {
    // std::cout << "Command: " << command << ", Arg: " << argument << "\n";

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
      if constexpr (adress_switching) {
        apply_adress_bitmask(argument, index);
      } else {
        memory[index] = apply_bitmask(argument, bitmask);
        // std::cout << "mem[" << index << "] = " << memory[index] << "\n";
      }
      return;
    }
  }

  void apply_adress_bitmask(const std::string &arg,
                            const std::size_t memory_index) {
    auto memory_mask = apply_bitmask_p2(memory_index, bitmask);
    // std::cout << "\nBitmask: \n" << bitmask << "\n";
    // std::cout << "\nMemory: \n" << memory_mask << "\n";

    std::vector<int> x_indices;
    for (int i = 0; i < memory_mask.size(); i++) {
      if (memory_mask.at(i) == 'X') {
        x_indices.push_back(i);
      }
    }
    std::vector<std::vector<int>> combinations =
        aoc_util::get_subsets(x_indices);
    // for (const auto &subset : combinations) {
    //   std::cout << "Subset: ";
    //   for (const auto &el : subset) {
    //     std::cout << el << " ";
    //   }
    //   std::cout << "\n";
    // }

    bitmasks.clear();
    std::string initial_mask = memory_mask;
    for (auto &el : initial_mask) {
      if (el == 'X') {
        el = '0';
      }
    }
    bitmasks.push_back(initial_mask);

    for (const auto &subset : combinations) {
      auto bitmask_copy = initial_mask;
      for (const auto &index : subset) {
        bitmask_copy.replace(index, 1, "1");
      }
      bitmasks.push_back(bitmask_copy);
    }

    //std::cout << "Bitmasks: \n";
    for (const auto &bitmask : bitmasks) {
      // std::cout << bitmask << "\n";
      auto memory_index = std::bitset<36>(bitmask).to_ullong();
      if (memory.size() < memory_index + 1) {
        memory.resize(memory_index + 1);
      }
      // std::cout << "Memory Index: " << memory_index << "\n";
      memory[memory_index] = std::stoll(arg);
    }
  }

  unsigned long long apply_bitmask(const std::string &arg,
                                   const std::string &bmask) {
    auto arg_bits = std::bitset<36>(std::stoll(arg));
    auto result_bits = std::bitset<36>(0);
    for (int i = 0; i < 36; i++) {
      auto curr_bitmask = bmask.at(i);

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

  std::string apply_bitmask_p2(const std::size_t &address,
                               const std::string &bmask) {
    auto result_bits = std::bitset<36>(address).to_string();
    for (int i = 0; i < 36; i++) {
      auto curr_bitmask = bmask.at(i);

      if (curr_bitmask == 'X') {
        result_bits.replace(i, 1, "X");
        continue;
      }

      if (curr_bitmask == '1') {
        result_bits.replace(i, 1, "1");
        continue;
      }

      if (curr_bitmask == '0') {
        continue;
      }
    }

    return result_bits;
  }

  unsigned long long sum() {
    unsigned long long res = 0;
    for (const auto &ull : memory) {
      res += ull;
    }
    return res;
  }
};

template <bool adress_switching>
unsigned long long run_commands(const std::string &input) {
  auto lines = aoc_util::get_lines(input);
  Computer c;
  for (const auto &line : lines) {
    std::vector<std::string> split;
    boost::split(split, line, boost::is_any_of(" "));
    c.apply_command<adress_switching>(split.at(0), split.at(2));
  }
  return c.sum();
}

unsigned long long part1(const std::string &input) {
  return run_commands<false>(input);
}

unsigned long long part2(const std::string &input) {
  return run_commands<true>(input);
}

int main() {
  std::cout << "[part1] test solution: " << part1("input14_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input14_2.txt") << "\n";
  std::cout << "[part2] test solution: " << part2("input14_3.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input14_2.txt") << "\n";
  return 0;
}