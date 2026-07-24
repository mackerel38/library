#include <cassert>
#include <utility>
#include <vector>

#include "structure/pointcount2d.hpp"

using namespace poe;

int main() {
    pointcount2d<int> points({{0, 0}, {1, 2}, {1, 2}, {-1, 3}});
    assert(points.size() == 4);
    assert(points[1] == std::pair(1, 2));
    assert(points.count(0, 0, 2, 3) == 3);
    assert(points.count_closed(-1, 2, 1, 3) == 3);
}
