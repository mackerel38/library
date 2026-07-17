#include "structure/orderstatistic.hpp"

int main() {
    poe::orderstatisticmultiset<long long> data({1, 3, 5, 7});
    data.add(5, 2);
    data.add(1);
    data.add(7);
    assert(data.size() == 4 && data.sum() == 18);
    assert(data.kth(2) == 5);
    assert(data.sum_smallest(3) == 11);
    assert(data.sum_largest(2) == 12);
    assert(data.clamp_sum(3, 6) == 19);
    assert(data.clamp_sum(8, 2) == 32);
    data.erase(5);
    assert(data.count(5) == 1 && data.count_less(6) == 2);
}
