#include "geometry/convexpath.hpp"

int main() {
    using poe::point;
    const std::vector<point<long long>> square{
        {1, 1}, {3, 1}, {3, 3}, {1, 3}
    };
    const long double around = poe::shortest_path_avoiding_convex_polygon(
        square, point<long long>{0, 2}, point<long long>{5, 2});
    assert(std::abs(around - (std::sqrt(2.0L) + 2 + std::sqrt(5.0L))) < 1e-12L);

    const long double direct = poe::shortest_path_avoiding_convex_polygon(
        square, point<long long>{0, 0}, point<long long>{4, 0});
    assert(std::abs(direct - 4) < 1e-12L);
}
