#include "math/burnside.hpp"
#include "math/modint.hpp"

int main() {
    using mint = poe::staticmodint<1000000007>;
    for (int size = 0; size <= 8; ++size) {
        const mint result = poe::symmetric_group_orbit_count<mint>(
            size,
            [](const std::vector<int>& cycle_counts) {
                int cycles = 0;
                for (const int count : cycle_counts) cycles += count;
                return mint{3}.pow(cycles);
            });
        assert(result == mint{(size + 2) * (size + 1) / 2});
    }
}
