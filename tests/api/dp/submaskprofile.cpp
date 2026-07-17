#include <cassert>
#include "dp/profile.hpp"

int main() {
    const auto result = poe::minimum_submask_profile(
        std::vector<std::uint64_t>{0b111, 0b111}, 3,
        [](std::uint64_t upper, std::uint64_t lower) {
            const auto common = upper & lower;
            return (common & (common << 1)) == 0;
        });
    assert(result.cost == 1);
    assert((result.masks[0] & result.masks[1]
            & ((result.masks[0] & result.masks[1]) << 1)) == 0);
}
