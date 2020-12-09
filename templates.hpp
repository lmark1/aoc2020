#include <iterator>
#include <vector>

template <typename T, typename It>
bool preamble_contains(It begin, It end, T val) {
  end--;
  while (begin != end) {

    auto res = *begin + *end;
  
    if (res == val) {
      return true;
    }

    if (res < val) {
      begin++;
      continue;
    }

    if (res > val) {
      end--;
      continue;
    }
  }

  return false;
}