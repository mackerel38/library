#include <cassert>
#include "structure/intervalset.hpp"

int main() {
    poe::intervalset<long long> intervals;
    assert(intervals.add(2, 8) == 6);
    assert(intervals.add(5, 10) == 2);
    assert(intervals.erase(4, 7) == 3);
    assert(intervals.covered() == 5);
    assert(intervals.contains(3));
    assert(!intervals.contains(5));
}
