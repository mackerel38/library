#include <cassert>
#include <vector>

#include "dp/segments.hpp"

int main() {
    assert(poe::maximum_k_disjoint_subarray_sum<long long>({1, -3, 4, -2, 5}, 0) == 0);
    assert(poe::maximum_k_disjoint_subarray_sum<long long>({1, -3, 4, -2, 5}, 1) == 7);
    assert(poe::maximum_k_disjoint_subarray_sum<long long>({1, -3, 4, -2, 5}, 2) == 9);
    assert(poe::maximum_k_disjoint_subarray_sum<long long>({-5, -1, -2}, 3) == 0);
}
