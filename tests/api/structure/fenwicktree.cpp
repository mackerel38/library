#include "structure/fenwicktree.hpp"

int main() {
    poe::fenwicktree<long long> sums(5);

    assert(sums.size() == 5);
    assert(sums.sum(0) == 0);
    assert(sums.sum(0, 5) == 0);

    sums.add(0, 4);
    sums.add(2, 7);
    sums.set(2, 5);
    sums.set(4, 3);

    assert(sums[0] == 4);
    assert(sums[1] == 0);
    assert(sums[2] == 5);
    assert(sums.sum(1) == 4);
    assert(sums.sum(3) == 9);
    assert(sums.sum(1, 4) == 5);
    assert(sums.sum(2, 2) == 0);
    assert(sums.sum(0, 5) == 12);

    poe::fenwicktree<int> frequencies(5);
    frequencies.set(0, 2);
    frequencies.set(1, 3);
    frequencies.set(3, 4);
    assert(frequencies.lower_bound(1) == 0);
    assert(frequencies.lower_bound(2) == 0);
    assert(frequencies.lower_bound(3) == 1);
    assert(frequencies.lower_bound(5) == 1);
    assert(frequencies.lower_bound(6) == 3);
    assert(frequencies.lower_bound(10) == 5);
    assert(frequencies.upper_bound(-1) == 0);
    assert(frequencies.upper_bound(0) == 0);
    assert(frequencies.upper_bound(2) == 1);
    assert(frequencies.upper_bound(5) == 3);
    assert(frequencies.upper_bound(9) == 5);

    poe::fenwicktree<int> empty(0);
    assert(empty.size() == 0);
    assert(empty.sum(0) == 0);
    assert(empty.sum(0, 0) == 0);
    assert(empty.lower_bound(1) == 0);
    assert(empty.upper_bound(0) == 0);
}
