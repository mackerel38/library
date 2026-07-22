#include <cassert>
#include <vector>

#include "algorithm/modularrange.hpp"

int main() {
    assert(poe::minimum_modular_range(std::vector<long long>{3, 21, 9}, 10LL) == 4);
    assert(poe::minimum_modular_range(std::vector<int>{-1, 1}, 10) == 2);
    assert(poe::minimum_modular_range(std::vector<int>{5}, 7) == 0);
}
