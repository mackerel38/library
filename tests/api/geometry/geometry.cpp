#include <cassert>

#include "geometry/all.hpp"

int main() {
    using point = poe::point<long long>;
    assert(poe::orientation(point{0, 0}, point{1, 0}, point{1, 1}) == 1);
    assert(poe::segments_intersect(point{0, 0}, point{2, 2}, point{0, 2}, point{2, 0}));
    assert(poe::convex_hull(std::vector<point>{{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}}).size() == 4);
    assert(poe::is_convex(std::vector<point>{{0, 0}, {1, 0}, {1, 1}, {0, 1}}));
    std::vector<point> directions{{0, -1}, {-1, 0}, {0, 1}, {1, 0}, {2, 0}};
    std::ranges::sort(directions, poe::argument_less<long long>);
    assert((directions == std::vector<point>{{1, 0}, {2, 0}, {0, 1}, {-1, 0}, {0, -1}}));
    assert(poe::same_direction(point{1, 2}, point{2, 4}));
    assert(std::abs(poe::distance_to_segment(
        point{1, 2}, point{0, 0}, point{2, 0}) - 2.0L) < 1e-12L);
    assert(std::abs(poe::minimum_distance_segment_walks(
        point{0, 0}, point{-2, 2}, point{-1, -1}, point{4, 4}) - 1.0L) < 1e-12L);

    const auto transform = poe::affine<long long>::reflect_x(3)
        * poe::affine<long long>::rotate_clockwise();
    assert(transform(point{1, 2}) == point(4, -1));

    const auto enclosing = poe::minimum_enclosing_circle(std::vector<point>{{0, 0}, {2, 0}, {0, 2}});
    assert(std::abs(enclosing.radius - std::sqrt(2.0L)) < 1e-10L);

    const std::vector<point> square{{0, 0}, {3, 0}, {3, 3}, {0, 3}};
    const poe::translatedpolygonintersection region(square, std::vector<point>{{0, 1}, {1, 0}});
    assert(region.contains({1, 1}));
    assert(!region.contains({0, 0}));
}
