#include "util.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <numeric>

int part1(const std::string &input) {
  auto lines = aoc_util::get_lines(input);

  std::vector<std::string> bus_ids;
  boost::split(bus_ids, lines.at(1), boost::is_any_of(","));

  std::vector<int> valid_ids;
  std::for_each(bus_ids.begin(), bus_ids.end(), [&](const auto &el) {
    if (el == "x") {
      return;
    }
    valid_ids.emplace_back(std::stoi(el));
  });

  auto timestamp = std::stoi(lines.at(0));
  double smallest_distance = 1e9;
  int best_id = -1;
  for (const auto &id : valid_ids) {
    double div = ceil(static_cast<double>(timestamp) / id) * id;
    if (div < smallest_distance) {
      smallest_distance = div;
      best_id = id;
    }
  }

  return best_id * (smallest_distance - timestamp);
}

struct id_fcn {
  int id_k_i_next;
  int id_k_i;
  int b;
  friend std::ostream &operator<<(std::ostream &o, const id_fcn &el) {

    o << "K(i+1) = " << el.id_k_i_next << ", K(i) = " << el.id_k_i
      << ", Inc: " << el.b << "\n";
    return o;
  }
};

long long get_next_k(const id_fcn &el, long long last_k) {
  while (true) {
    last_k++;
    long long temp = el.id_k_i * last_k + el.b;
    if (temp % el.id_k_i_next == 0) {
      return last_k;
    }
  }
}

long long brute_force(const std::vector<id_fcn> &fcn_vector) {

  const auto first = fcn_vector.front();
  long long multi = 0;
  long long counter = 0;

  while (true) {
    multi = get_next_k(first, multi);
    long long next_k = (multi * first.id_k_i + first.b) / first.id_k_i_next;

    const auto is_integer = [&](const id_fcn &el) {
      long long temp = el.id_k_i * next_k + el.b;
      if (temp % el.id_k_i_next != 0) {
        return false;
      }
      next_k = temp / el.id_k_i_next;
      return true;
    };

    auto it =
        std::find_if_not(fcn_vector.begin() + 1, fcn_vector.end(), is_integer);
    if (it == fcn_vector.end()) {
      return multi * first.id_k_i;
    }
    counter++;
  }

  return 0;
}

long long modular_multiplicative_inverse(long long a, long long m) {
  long long m0 = m, t, q;
  long long x0 = 0, x1 = 1;

  if (m == 1)
    return 0;

  // Apply extended Euclid Algorithm
  while (a > 1) {
    // q is quotient
    q = a / m;

    t = m;

    // m is remainder now, process same as
    // euclid's algo
    m = a % m, a = t;

    t = x0;

    x0 = x1 - q * x0;

    x1 = t;
  }

  // Make x1 positive
  if (x1 < 0)
    x1 += m0;

  return x1;
}

long long chinese_remainder_theorem(const std::vector<id_fcn> &fcn_vector) {
  std::vector<long long> num, rem;
  int mod_sum = 0;
  num.push_back(fcn_vector.back().id_k_i_next);
  rem.push_back(0);

  std::for_each(fcn_vector.rbegin(), fcn_vector.rend(), [&](const auto &el) {
    mod_sum += el.b;
    num.push_back(el.id_k_i);
    rem.push_back(mod_sum);
  });
  auto prod =
      std::accumulate(num.begin(), num.end(), 1L, std::multiplies<long long>());

  long long result = 0;
  for (auto i = 0; i < num.size(); i++) {
    auto pp = prod / num[i];
    auto inv = modular_multiplicative_inverse(pp, num[i]);
    result += rem[i] * inv * pp;
  }
  result %= prod;
  return result - rem.back();
}

long long part2(const std::string &input) {
  auto lines = aoc_util::get_lines(input);

  std::vector<std::string> bus_ids;
  boost::split(bus_ids, lines.at(1), boost::is_any_of(","));

  int id_k_i = -1;
  int increment = 0;
  std::vector<id_fcn> fcn_vector;
  for (const auto &id : bus_ids) {

    if (id == "x") {
      increment++;
      continue;
    }

    auto id_k_i_1 = std::stoi(id);
    if (id_k_i < 0) {
      id_k_i = id_k_i_1;
      continue;
    }

    increment++;
    fcn_vector.push_back({id_k_i_1, id_k_i, increment});

    // Reset
    increment = 0;
    id_k_i = id_k_i_1;
  }
  // return brute_force(fcn_vector);
  return chinese_remainder_theorem(fcn_vector);
}

int main() {
  std::cout << "[part1] test solution: " << part1("input13_1.txt") << "\n";
  std::cout << "[part1] test solution: " << part1("input13_2.txt") << "\n";
  std::cout << "[part2] test solution: " << part2("input13_1.txt") << "\n";
  std::cout << "[part2] test solution 1: " << part2("input13_3.txt") << "\n";
  std::cout << "[part2] test solution 2: " << part2("input13_4.txt") << "\n";
  std::cout << "[part2] test solution 3: " << part2("input13_5.txt") << "\n";
  std::cout << "[part2] test solution 4: " << part2("input13_6.txt") << "\n";
  std::cout << "[part2] test solution 5: " << part2("input13_7.txt") << "\n";
  std::cout << "[part2] solution: " << part2("input13_2.txt") << "\n";
  return 0;
}