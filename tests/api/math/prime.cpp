#include <cassert>

#include "math/prime.hpp"

int main() {
    poe::primetable table(100);
    assert(table[97] && !table[91]);
    assert(table.least(91) == 7);
    assert((table.factorize(84) == std::vector<std::pair<int, int>>{{2, 2}, {3, 1}, {7, 1}}));
    assert((table.divisors(12) == std::vector<int>{1, 2, 3, 4, 6, 12}));
    assert(poe::is_prime(998244353));
    assert((poe::factorize(1000000007ULL * 1000000009ULL).size() == 2));
    assert((poe::signed_divisors(-12) ==
            std::vector<long long>{-12, -6, -4, -3, -2, -1, 1, 2, 3, 4, 6, 12}));
}
