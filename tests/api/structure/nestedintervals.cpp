#include <cassert>
#include <utility>
#include <vector>

#include "structure/nestedintervals.hpp"

int main() {
    poe::nestedintervals<long long> intervals({2, 4, 6, 10});
    intervals.align_right(2);
    intervals.align_left(1);
    assert(intervals[0] == std::pair(2LL, 4LL));
    assert(intervals.interval(2) == std::pair(0LL, 6LL));
    assert(intervals.count_covering(2) == 4);
    intervals.set_prefix_right(3, 10);
    assert(intervals.count_covering(1) == 1);
}
