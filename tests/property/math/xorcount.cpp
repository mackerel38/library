#include "math/modint.hpp"
#include "math/xorcount.hpp"

#include <cassert>
#include <vector>

int main() {
    using mint = poe::modint998244353;
    for (int alphabet = 1; alphabet <= 6; ++alphabet) {
        for (int target = 0; target < 8; ++target) {
            constexpr int maximum_length = 6;
            std::vector<mint> tuple_expected(maximum_length + 1);
            std::vector<mint> distinct_expected(maximum_length + 1);
            auto enumerate = [&](auto&& self, int length, int index, int value,
                                 unsigned used) -> void {
                if (index == length) {
                    if (value == target) {
                        ++tuple_expected[length];
                        if (std::popcount(used) == length) ++distinct_expected[length];
                    }
                    return;
                }
                for (int next = 0; next < alphabet; ++next) {
                    self(self, length, index + 1, value ^ next, used | (1U << next));
                }
            };
            for (int length = 0; length <= maximum_length; ++length) {
                enumerate(enumerate, length, 0, 0, 0);
            }

            const auto tuple = poe::bounded_xor_tuple_counts<mint>(
                maximum_length, alphabet - 1, target
            );
            assert(tuple == tuple_expected);
            const auto distinct = poe::distinct_xor_tuple_counts(tuple, alphabet);
            assert(distinct == distinct_expected);

            for (int length = 0; length <= maximum_length; ++length) {
                mint expected = 0;
                auto multiset = [&](auto&& self, int remaining, int minimum,
                                    int value) -> void {
                    if (remaining == 0) {
                        if (value == target) ++expected;
                        return;
                    }
                    for (int next = minimum; next < alphabet; ++next) {
                        self(self, remaining - 1, next, value ^ next);
                    }
                };
                multiset(multiset, length, 0, 0);
                assert(poe::xor_multiset_count(length, alphabet, distinct) == expected);
            }
        }
    }
}
