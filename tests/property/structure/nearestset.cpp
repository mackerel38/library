#include <cassert>
#include <random>
#include <set>
#include "structure/nearestset.hpp"

int main() {
    std::mt19937 random(430);
    for (int trial = 0; trial < 150; ++trial) {
        poe::nearestset<int> points;
        std::set<int> values;
        for (int operation = 0; operation < 600; ++operation) {
            const int value = static_cast<int>(random() % 101) - 50;
            if (random() & 1) assert(points.add(value) == values.insert(value).second);
            else assert(points.erase(value) == (values.erase(value) != 0));
            long long expected = 0;
            for (auto iterator = values.begin(); iterator != values.end(); ++iterator) {
                const int current = *iterator;
                int nearest = 1'000'000;
                if (iterator != values.begin()) nearest = std::min(nearest, current - *std::prev(iterator));
                if (std::next(iterator) != values.end()) {
                    nearest = std::min(nearest, *std::next(iterator) - current);
                }
                if (values.size() >= 2) {
                    expected += nearest;
                    assert(points.distance(current) == nearest);
                } else {
                    assert(!points.distance(current));
                }
            }
            assert(points.sum() == expected);
            assert(points.values() == values);
        }
    }
}
