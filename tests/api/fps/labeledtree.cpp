#include "fps/combinatorial.hpp"

using namespace std;
using namespace poe;

int main() {
    assert(count_labeled_trees_with_degrees<998244353>(4, {1, 3}) == 4);
    assert(count_labeled_trees_with_degrees<998244353>(5, {1, 2}) == 60);
    assert(count_labeled_trees_with_degrees<998244353>(2, {1}) == 1);

    vector<modint998244353> weight(4);
    weight[1] = 2;
    weight[2] = 3;
    weight[3] = 5;
    assert(labeled_tree_degree_weight_sum<998244353>(4, weight) == 592);
}
