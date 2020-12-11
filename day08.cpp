#include "opcode_parser.hpp"
#include "util.hpp"

int part1(const std::string& input)
{
   auto lines = aoc_util::get_lines(input);
   aoc_util::opcode o;
   o.parse(lines);
   return o.run().result;
}

bool toggle_line(std::string& line)
{
  bool line_changed = false;
  if (line.substr(0, 3) == "jmp") {
    line_changed = true;
    line.replace(0, 3, "nop");
  }
  else if (line.substr(0, 3) == "nop") {
    line_changed = true;
    line.replace(0, 3, "jmp");
  }
  return line_changed;
}

int part2(const std::string& input)
{
  auto lines = aoc_util::get_lines(input);
  aoc_util::opcode o;

  for(auto& line : lines)
  {
    bool line_changed = toggle_line(line);
    if (line_changed)
    {
      o.parse(lines);
      auto res = o.run();
      if (!res.is_infinite_loop)
      {
        return res.result;
      }
      o.reset();
      toggle_line(line);
    }
  }
  return 0;
}

int main()
{
  std::cout << "[part1] test solution: " << part1("input08_1.txt") << "\n";
  std::cout << "[part1] solution: " << part1("input08_2.txt") << "\n";
  std::cout << "[part2] test solution: " << part2("input08_1.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input08_2.txt") << "\n";
  return 0;
}