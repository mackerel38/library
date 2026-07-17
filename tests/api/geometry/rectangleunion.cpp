#include <cassert>
#include <vector>

#include "geometry/rectangleunion.hpp"

int main() {
    using rectangle = poe::axisalignedrectangle<long long>;
    const rectangle base{0, 3, 0, 2};
    assert(poe::rectangle_area(base) == 6);
    const auto split = poe::split_rectangle_x(base, 1LL);
    assert(split.size() == 2 && poe::rectangle_area(split[0]) == 2 && poe::rectangle_area(split[1]) == 4);
    assert(poe::translate_rectangle(base, 4LL, -1LL).left == 4);
    assert(poe::rectangles_edge_adjacent(base, rectangle{3, 5, 1, 4}));
    assert(!poe::rectangles_edge_adjacent(base, rectangle{3, 5, 2, 4}));
    assert(poe::rectangle_union_area<long long>({rectangle{0, 3, 0, 2},
                                                  rectangle{2, 4, 1, 3}}) == 9);
    assert(poe::rectangle_union_area<long long>({rectangle{1, 1, 0, 5}}) == 0);
}
