#include "dp/subsetsumcount.hpp"

using namespace poe;

int main() {
    const auto count = subset_sum_parity_counts<998244353>({1, 2, 2}, 5);
    assert(count.even[0] == 1);
    assert(count.odd[1] == 1);
    assert(count.odd[2] == 2);
    assert(count.even[3] == 2);
    assert(count.even[4] == 1);
    assert(count.odd[5] == 1);

    const auto with_zero = subset_sum_parity_counts<998244353>({0, 1}, 1);
    assert(with_zero.even[0] == 1 && with_zero.odd[0] == 1);
    assert(with_zero.even[1] == 1 && with_zero.odd[1] == 1);
}
