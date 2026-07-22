#include "geometry/line.hpp"

using namespace poe;

int main() {
    const line<int> first{1, 1, -3};
    const line<int> second{1, -1, -1};
    const auto intersection = line_intersection(first, second);
    assert(intersection.has_value());
    assert(std::abs(intersection->x - 2) < 1e-12L);
    assert(std::abs(intersection->y - 1) < 1e-12L);
    assert(std::abs(distance_to_line(point<int>{0, 0}, first) - 3 / std::sqrt(2.0L)) < 1e-12L);
    assert(!line_intersection(first, line<int>{2, 2, 5}).has_value());
}
