#include <bits/stdc++.h>
#include "algorithm/intervalcover.hpp"

using namespace std;
using namespace poe;

int main() {
    const vector<pair<int, int>> intervals = {{0, 1}, {0, 2}, {1, 3}};
    const auto count = interval_cover_subset_counts(3, intervals);
    assert(count[0].val() == 0);
    assert(count[1].val() == 0);
    assert(count[2].val() == 2);
    assert(count[3].val() == 1);
    assert(expected_random_interval_cover_time(3, intervals).val() == 499122180);
}
