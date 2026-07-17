#include <cassert>

#include "fps/implicit.hpp"

int main() {
    using series = poe::fps998244353;
    for (int size = 1; size <= 100; ++size) {
        const series value = poe::newton_fps<998244353>(size, 0,
            [&](const series& current, int length) {
                series shifted(length);
                const series exponential = current.exp(length - 1);
                for (int i = 1; i < length; ++i) shifted[i] = exponential[i - 1];
                series derivative = shifted;
                derivative[0] -= 1;
                return std::pair{shifted - current, derivative};
            });
        series shifted(size);
        const series exponential = value.exp(std::max(0, size - 1));
        for (int i = 1; i < size; ++i) shifted[i] = exponential[i - 1];
        assert(value == shifted);
    }
}
