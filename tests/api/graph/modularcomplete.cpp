#include <cassert>
#include <vector>

#include "graph/modularcomplete.hpp"

using namespace poe;

int main() {
    const auto distance = modular_complete_distances<long long>(
        {10, 11, 6, 0}, {8, 7, 4, 1}, 12
    );
    assert(distance == std::vector<long long>({0, 3, 2, 3}));
}
