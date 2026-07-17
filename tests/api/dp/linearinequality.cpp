#include <cassert>
#include "dp/linearinequality.hpp"
int main() {
    assert(poe::count_nonnegative_weighted_sum_at_most(std::vector<int>{5, 4, 3, 2}, 6).val() == 10);
    assert(poe::count_nonnegative_weighted_sum_at_most(std::vector<int>{1}, 1'000'000'007ULL).val() == 1'755'655);
}
