#include <cassert>

#include "geometry/manhattan.hpp"

using namespace poe;

int main() {
    manhattannearest near(std::vector<std::pair<long long, long long>>{{3, 3}, {4, 6}, {7, 4}, {2, 5}});
    assert(near.size() == 4);
    assert(near[0] == std::pair(3LL, 3LL));
    assert(near.count(3, 5, 2) == 3);
    assert(near.kth(3, 5, 1) == 1);
    assert(near.kth(3, 5, 4) == 5);
}
