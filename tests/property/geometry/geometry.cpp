#include <cassert>
#include <random>

#include "geometry/all.hpp"

int main() {
    using point = poe::point<long long>;
    std::mt19937 random(20260722);
    const std::vector<point> base{{-3, -2}, {2, -2}, {4, 1}, {1, 4}, {-3, 3}};
    for (int test = 0; test < 1000; ++test) {
        std::vector<point> shifts(1 + random() % 8);
        for (auto& shift : shifts) {
            shift = {static_cast<int>(random() % 11) - 5,
                     static_cast<int>(random() % 11) - 5};
        }
        const poe::translatedpolygonintersection region(base, shifts);
        for (int query = 0; query < 100; ++query) {
            const point target{static_cast<int>(random() % 31) - 15,
                               static_cast<int>(random() % 31) - 15};
            bool expected = true;
            for (const point shift : shifts) {
                expected &= poe::polygon_contains(base, target - shift) != poe::containment::outside;
            }
            assert(region.contains(target) == expected);
        }
    }
    for (int test = 0; test < 200; ++test) {
        std::vector<point> points(1 + random() % 12);
        for (auto& value : points) value = {static_cast<int>(random() % 21) - 10, static_cast<int>(random() % 21) - 10};
        const auto hull = poe::convex_hull(points);
        for (const auto& value : points) {
            if (hull.size() == 1) assert(value == hull[0]);
            else if (hull.size() == 2) assert(poe::on_segment(hull[0], hull[1], value));
            else assert(poe::polygon_contains(hull, value) != poe::containment::outside);
        }

        const auto circle = poe::minimum_enclosing_circle(points);
        for (const auto& value : points) assert(circle.contains(value));
        long double brute = std::numeric_limits<long double>::infinity();
        for (const auto& first : points) {
            poe::circle candidate{{static_cast<long double>(first.x), static_cast<long double>(first.y)}, 0};
            bool ok = true;
            for (const auto& value : points) ok &= candidate.contains(value);
            if (ok) brute = std::min(brute, candidate.radius);
        }
        for (const auto& first : points) for (const auto& second : points) {
            const auto candidate = poe::detail::diameter_circle(first, second);
            bool ok = true;
            for (const auto& value : points) ok &= candidate.contains(value);
            if (ok) brute = std::min(brute, candidate.radius);
        }
        for (const auto& first : points) for (const auto& second : points) for (const auto& third : points) {
            const auto candidate = poe::detail::circumcircle(first, second, third);
            bool ok = true;
            for (const auto& value : points) ok &= candidate.contains(value);
            if (ok) brute = std::min(brute, candidate.radius);
        }
        assert(std::abs(circle.radius - brute) < 1e-8L);
    }

    for (int test = 0; test < 500; ++test) {
        point first_start, first_goal, second_start, second_goal;
        do {
            first_start = {static_cast<int>(random() % 21) - 10,
                           static_cast<int>(random() % 21) - 10};
            first_goal = {static_cast<int>(random() % 21) - 10,
                          static_cast<int>(random() % 21) - 10};
        } while (first_start == first_goal);
        do {
            second_start = {static_cast<int>(random() % 21) - 10,
                            static_cast<int>(random() % 21) - 10};
            second_goal = {static_cast<int>(random() % 21) - 10,
                           static_cast<int>(random() % 21) - 10};
        } while (second_start == second_goal);

        const long double answer = poe::minimum_distance_segment_walks(
            first_start, first_goal, second_start, second_goal);
        const long double first_time = poe::distance(first_start, first_goal);
        const long double second_time = poe::distance(second_start, second_goal);
        const long double end_time = std::max(first_time, second_time);
        long double sampled = std::numeric_limits<long double>::infinity();
        for (int step = 0; step <= 20000; ++step) {
            const long double time = end_time * step / 20000;
            const auto position = [time](point start, point goal, long double finish) {
                const long double ratio = std::min(1.0L, time / finish);
                return poe::point<long double>{
                    start.x + (goal.x - start.x) * ratio,
                    start.y + (goal.y - start.y) * ratio};
            };
            sampled = std::min(sampled, poe::distance(
                position(first_start, first_goal, first_time),
                position(second_start, second_goal, second_time)));
        }
        assert(answer <= sampled + 1e-10L);
        assert(sampled - answer <= end_time / 10000 + 1e-9L);
    }
}
