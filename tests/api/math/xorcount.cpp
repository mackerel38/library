#include "math/modint.hpp"
#include "math/xorcount.hpp"

#include <cassert>

int main() {
    using mint = poe::modint998244353;
    const auto tuple = poe::bounded_xor_tuple_counts<mint>(3, 3, 2);
    const auto distinct = poe::distinct_xor_tuple_counts(tuple, 4);
    assert(poe::xor_multiset_count(3, 4, distinct) == 5);
}
