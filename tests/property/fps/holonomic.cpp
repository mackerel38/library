#include <cassert>
#include <optional>

#include "fps/holonomic.hpp"
#include "math/modint.hpp"

int main() {
    using mint = poe::modint998244353;
    const auto fibonacci = poe::holonomic_sequence<mint>(100, 2,
        [](int, int distance) { return distance == 0 ? mint{1} : mint{-1}; },
        [](int index, const auto&) -> std::optional<mint> {
            if (index < 2) return mint{index};
            return std::nullopt;
        });
    for (int i = 2; i < 100; ++i) assert(fibonacci[i] == fibonacci[i - 1] + fibonacci[i - 2]);
}
