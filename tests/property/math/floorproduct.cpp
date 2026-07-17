#include <cassert>

#include "math/floorproduct.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    using monoid = powersummonoid<mint>;
    for (unsigned long long n = 0; n <= 20; ++n) {
        for (unsigned long long m = 1; m <= 20; ++m) {
            for (unsigned long long a = 0; a <= 25; ++a) {
                for (unsigned long long b = 0; b <= 25; ++b) {
                    const mint first = a + 2;
                    const mint second = b + 3;
                    mint expected = 0;
                    for (unsigned long long i = 0; i < n; ++i) {
                        expected += first.pow(i) * second.pow((a * i + b) / m);
                    }
                    const auto result = floor_product(n, m, a, b, monoid::x(first), monoid::y(second));
                    assert(result.answer() == expected);
                }
            }
        }
    }
}
