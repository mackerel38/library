#include "dp/decreasingconvex.hpp"

int main() {
    poe::decreasingconvexsequence<long long> left(4, 10);
    poe::decreasingconvexsequence<long long> right(2, 20);
    left.merge(std::move(right));
    assert(left[0] == 80 && left[2] == 20 && left[4] == 0);
    left.suffix_min(15, 3);
    assert(left[0] == 55 && left.zero() == 55);
    assert(left[3] == 10 && left[4] == 0);
}
