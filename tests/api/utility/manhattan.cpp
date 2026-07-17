#include <cassert>
#include <vector>
#include "utility/presetsegtree.hpp"
int main() {
    poe::point_set_range_manhattan_max<long long> data({{-1, -1}, {1, 2}, {-2, 1}});
    assert(data.max(0, 2, 0, 0) == 3);
    data.set(0, 0, 1);
    assert(data.max(0, 3, -1, 2) == 2);
}
