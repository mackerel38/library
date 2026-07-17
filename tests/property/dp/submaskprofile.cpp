#include <cassert>
#include "dp/profile.hpp"

int main() {
    for (int height = 1; height <= 4; ++height) {
        for (int width = 1; width <= 4; ++width) {
            if (height * width > 10) continue;
            const int count = 1 << width;
            for (int pattern = 0; pattern < (1 << (height * width)); ++pattern) {
                std::vector<std::uint64_t> available(height);
                for (int row = 0; row < height; ++row) {
                    available[row] = pattern >> (row * width) & (count - 1);
                }
                const auto compatible = [](std::uint64_t upper, std::uint64_t lower) {
                    const auto common = upper & lower;
                    return (common & (common << 1)) == 0;
                };
                const auto result = poe::minimum_submask_profile(
                    available, width, compatible);

                int expected = height * width;
                const auto search = [&](auto&& self, int row, std::uint64_t before,
                                        int removed) -> void {
                    if (row == height) {
                        expected = std::min(expected, removed);
                        return;
                    }
                    for (std::uint64_t mask = available[row];;
                         mask = (mask - 1) & available[row]) {
                        if (row == 0 || compatible(before, mask)) {
                            self(self, row + 1, mask,
                                 removed + std::popcount(available[row] ^ mask));
                        }
                        if (mask == 0) break;
                    }
                };
                search(search, 0, 0, 0);
                assert(result.cost == expected);
                for (int row = 0; row < height; ++row) {
                    assert((result.masks[row] | available[row]) == available[row]);
                    if (row) assert(compatible(result.masks[row - 1], result.masks[row]));
                }
            }
        }
    }
}
