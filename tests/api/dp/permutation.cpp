#include <cassert>
#include <vector>

#include "dp/permutation.hpp"

int main() {
    const auto all = poe::insertion_component_counts<long long>(
        4, [](int, int, int) { return true; }
    );
    assert(all[1] == 24);
    for (int components = 2; components <= 4; ++components) assert(all[components] == 0);

    const auto isolated = poe::insertion_component_counts<long long>(
        4, [](int, int, int joined) { return joined == 0; }
    );
    for (long long count : isolated) assert(count == 0);
}
