#include "algorithm/subarrayminsum.hpp"

int main() {
    assert(poe::count_subarrays_min_plus_sum_at_most(
        std::vector<long long>{9, 2, 6, 5},
        std::vector<long long>{3, 5, 8, 9}, 15LL) == 6);
    assert(poe::count_subarrays_min_plus_sum_at_most(
        std::vector<long long>{39, 9, 36, 94, 40, 26, 12, 26, 28, 66, 73, 85, 62, 5, 20},
        std::vector<long long>{0, 0, 7, 7, 0, 5, 5, 0, 7, 9, 9, 4, 2, 5, 2}, 100LL) == 119);
}
