#include "tree/perfectsubtree.hpp"

using namespace poe;

int main() {
    const std::vector<int> parent{-1, 0, 0, 1, 0, 1, 4, 4, 4, 0};
    const auto count = perfect_ary_subtree_prefix_counts<998244353>(parent);
    assert((count == std::vector<modint998244353>{1, 1, 2, 2, 4, 4, 4, 5, 7, 10}));

    assert((perfect_ary_subtree_prefix_counts<998244353>({-1}, 3) ==
            std::vector<modint998244353>{1}));
}
