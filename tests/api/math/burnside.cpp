#include "math/burnside.hpp"
#include "math/modint.hpp"

int main() {
    using mint = poe::staticmodint<1000000007>;
    const mint binary_multisets = poe::symmetric_group_orbit_count<mint>(
        3,
        [](const std::vector<int>& cycle_counts) {
            int cycles = 0;
            for (const int count : cycle_counts) cycles += count;
            return mint{2}.pow(cycles);
        });
    assert(binary_multisets == mint{4});
}
