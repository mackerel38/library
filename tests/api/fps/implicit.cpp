#include <cassert>

#include "fps/implicit.hpp"

int main() {
    using series = poe::fps998244353;
    const int size = 20;
    const series rooted_tree = poe::newton_fps<998244353>(size, 0,
        [&](const series& value, int length) {
            series shifted(length);
            const series exponential = value.exp(length - 1);
            for (int i = 1; i < length; ++i) shifted[i] = exponential[i - 1];
            series derivative = shifted;
            derivative[0] -= 1;
            return std::pair{shifted - value, derivative};
        });
    assert(rooted_tree[1] == 1);
    assert(rooted_tree[2] == 1);
    assert(rooted_tree[3] * 6 == 9);
}
