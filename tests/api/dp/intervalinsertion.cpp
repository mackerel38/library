#include <bits/stdc++.h>
#include "dp/intervalinsertion.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    const auto aggregate = interval_insertion_aggregate<mint>(
        4, 3,
        [](int left, int middle, int right) {
            return intervalinsertionchoice<mint>{1, (middle - left) * (right - middle)};
        }
    );
    assert(aggregate.count_at(0, 3) == 2);
    assert(aggregate.cost_at(0, 3) == 6);
    assert(circular_directional_removal_expectation<mint>("LLR") == mint{17} / 6);
}
