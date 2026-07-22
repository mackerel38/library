#include "geometry/convexpath.hpp"

namespace {

using poe::point;

bool segment_enters_interior(
    const std::vector<point<long long>>& polygon,
    const point<long long>& first,
    const point<long long>& second
) {
    long double lower = 0;
    long double upper = 1;
    for (int i = 0; i < static_cast<int>(polygon.size()); ++i) {
        const auto edge = polygon[(i + 1) % polygon.size()] - polygon[i];
        const auto from = first - polygon[i];
        const auto direction = second - first;
        const long double constant = static_cast<long double>(poe::cross(edge, from));
        const long double coefficient = static_cast<long double>(poe::cross(edge, direction));
        if (coefficient == 0) {
            if (constant <= 0) return false;
            continue;
        }
        const long double border = -constant / coefficient;
        if (coefficient > 0) lower = std::max(lower, border);
        else upper = std::min(upper, border);
    }
    return lower + 1e-15L < upper && upper > 0 && lower < 1;
}

long double brute(
    const std::vector<point<long long>>& polygon,
    const point<long long>& start,
    const point<long long>& goal
) {
    std::vector<point<long long>> nodes = polygon;
    nodes.push_back(start);
    nodes.push_back(goal);
    const int n = static_cast<int>(nodes.size());
    const long double inf = 1e100L;
    std::vector distance(n, std::vector<long double>(n, inf));
    for (int i = 0; i < n; ++i) {
        distance[i][i] = 0;
        for (int j = 0; j < i; ++j) {
            if (!segment_enters_interior(polygon, nodes[i], nodes[j])) {
                distance[i][j] = distance[j][i] = poe::distance(nodes[i], nodes[j]);
            }
        }
    }
    for (int middle = 0; middle < n; ++middle) {
        for (int first = 0; first < n; ++first) {
            for (int second = 0; second < n; ++second) {
                distance[first][second] = std::min(
                    distance[first][second],
                    distance[first][middle] + distance[middle][second]);
            }
        }
    }
    return distance[n - 2][n - 1];
}

}

int main() {
    std::mt19937 random(20260718);
    for (int trial = 0; trial < 500; ++trial) {
        std::vector<point<long long>> candidates;
        for (int i = 0; i < 12; ++i) {
            candidates.push_back({
                static_cast<long long>(random() % 17) - 8,
                static_cast<long long>(random() % 17) - 8
            });
        }
        const auto polygon = poe::convex_hull(candidates);
        if (polygon.size() < 3 || !poe::is_convex(polygon)) continue;

        auto outside_point = [&] {
            while (true) {
                point<long long> value{
                    static_cast<long long>(random() % 61) - 30,
                    static_cast<long long>(random() % 61) - 30
                };
                if (poe::polygon_contains(polygon, value) == poe::containment::outside) {
                    return value;
                }
            }
        };
        const auto start = outside_point();
        const auto goal = outside_point();
        const long double expected = brute(polygon, start, goal);
        const long double actual = poe::shortest_path_avoiding_convex_polygon(
            polygon, start, goal);
        assert(std::abs(expected - actual) <= 1e-10L * std::max(1.0L, expected));
    }
}
