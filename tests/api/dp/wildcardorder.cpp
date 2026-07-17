#include <cassert>

#include "dp/wildcardorder.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    assert(poe::count_strictly_increasing_wildcard_numbers<998244353>(
               {"?0", "??", "05"}) == mint{4});
    assert(poe::count_strictly_increasing_wildcard_numbers<998244353>(
               {"0", "0"}) == mint{0});
    assert(poe::count_strictly_increasing_wildcard_sequences<mint>(
               {{-1}, {-1}}, 3) == mint{3});
}
