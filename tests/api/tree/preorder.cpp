#include <cassert>
#include <vector>

#include "math/modint.hpp"
#include "tree/preorder.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    assert(poe::count_sorted_child_preorder_trees<mint>(std::vector<int>{1, 2, 4, 3}).val() == 3);
    assert(poe::count_sorted_child_preorder_trees<mint>(std::vector<int>{1}).val() == 1);
}
