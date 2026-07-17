#include <cassert>
#include <optional>

#include "fps/holonomic.hpp"

int main() {
    const auto factorial = poe::holonomic_sequence<long long>(10, 1,
        [](int index, int distance) {
            return distance == 0 ? 1LL : -static_cast<long long>(index);
        },
        [](int index, const auto&) -> std::optional<long long> {
            if (index == 0) return 1;
            return std::nullopt;
        });
    assert(factorial[9] == 362880);
}
