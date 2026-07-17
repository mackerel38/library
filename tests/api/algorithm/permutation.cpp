#include <cassert>
#include <vector>

#include "algorithm/permutation.hpp"

using namespace std;
using namespace poe;

int main() {
    assert(count_permutations_max_drop<998244353>({5, 2, 1, 2}, 1).val() == 3);
    assert(count_permutations_max_drop<998244353>({20, 40, 60, 80, 100}, 10).val() == 1);

    const vector<int> values{1, 2, 1, 2};
    const auto pairs = one_swap_lexicographic_pairs(values, {0, 2, 4});
    assert(pairs[0] == pair(1, 2));
    vector<int> middle = values;
    ranges::swap(middle[pairs[1].first], middle[pairs[1].second]);
    assert(middle == values);
    assert(pairs[2] == pair(0, 1));

    const auto positions = path_order_positions(string("RLLR"), [](char relation) {
        return relation == 'R';
    });
    assert(positions.counts == vector<int>({2, 4, 3, 4, 2}));
}
