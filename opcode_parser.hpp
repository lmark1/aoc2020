#include <boost/algorithm/string.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace aoc_util {

struct opcode_res {
  int result;
  bool is_infinite_loop;
};

class opcode {

public:
  using InstructionPair = std::pair<std::string, int>;

  opcode() {
    m_function_map.emplace("nop", [&](int n) { m_program_counter++; });
    m_function_map.emplace("jmp", [&](int n) { m_program_counter += n; });
    m_function_map.emplace("acc", [&](int n) {
      m_accumulator += n;
      m_program_counter++;
    });
  }

  void parse(const std::vector<std::string> &lines) {

    const auto add_to_instruction_set = [&](const auto &line) {
      m_instruction_set.emplace_back(parse_instruction(line));
    };
    std::for_each(lines.begin(), lines.end(), add_to_instruction_set);
  }

  void reset() {
    m_accumulator = 0;
    m_program_counter = 0;
    m_instruction_set.clear();
  }

  opcode_res run() {
    std::unordered_map<int, int> m_program_counter_map;
    bool infinite_loop = false;

    while (m_program_counter < m_instruction_set.size()) {
      m_program_counter_map.emplace(m_program_counter, 0);
      m_program_counter_map[m_program_counter]++;
      if (m_program_counter_map[m_program_counter] > 1) {
        infinite_loop = true;
        break;
      }

      auto instruction = m_instruction_set[m_program_counter].first;
      auto argument = m_instruction_set[m_program_counter].second;
      m_function_map[instruction](argument);
    }

    return {m_accumulator, infinite_loop};
  }

private:
  InstructionPair parse_instruction(const std::string &line) {
    std::vector<std::string> res;
    boost::split(res, line, boost::is_any_of(" "));
    return std::make_pair(res[0], std::stoi(res[1]));
  }

  std::vector<InstructionPair> m_instruction_set;
  std::unordered_map<std::string, std::function<void(int)>> m_function_map;

  int m_program_counter = 0;
  int m_accumulator = 0;
};

} // namespace aoc_util
