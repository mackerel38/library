#include <cassert>
#include <vector>
#include "algorithm/slopeconstraint.hpp"

using namespace poe;

int main() {
    assert((slope_minorant(std::vector<int>{5, 2, 1, 3, 4}, 1) ==
            std::vector<int>{3, 2, 1, 2, 3}));
    assert((slope_minorant(std::vector<int>{10, 10, 1}, 2) ==
            std::vector<int>{5, 3, 1}));
}
