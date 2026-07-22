#include "geometry/latticemove.hpp"

using namespace poe;

int main() {
    assert(minimum_lattice_moves({5, 3}, {1, 0}, {1, 1}) == 5);
    assert(!minimum_lattice_moves({0, 1}, {1, 0}, {-1, 0}));
    assert(minimum_lattice_moves(
        {std::numeric_limits<long long>::min(), 0},
        {std::numeric_limits<long long>::min(), 0}, {0, 0}) == 1);
    assert(!minimum_lattice_moves(
        {std::numeric_limits<long long>::min(), 0}, {-1, 0}, {0, 0}));

    const std::vector<point<long long>> steps{{2, 0}, {0, 3}, {1, 1}};
    assert(minimum_two_kind_lattice_moves({4, 6}, steps) == 4);
    assert(!minimum_two_kind_lattice_moves({1, 0}, std::span<const point<long long>>{}));
}
