#include <cassert>
#include "structure/nearestset.hpp"

int main() {
    poe::nearestset<int> points;
    assert(points.add(0));
    assert(points.sum() == 0);
    assert(points.add(5));
    assert(points.sum() == 10);
    assert(points.add(2));
    assert(points.sum() == 7);
    assert(points.distance(2) == 2);
    assert(points.erase(2));
    assert(points.sum() == 10);
    assert(!points.erase(2) && !points.add(5));
}
