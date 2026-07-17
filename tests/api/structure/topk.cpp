#include <cassert>

#include "structure/segtree.hpp"
#include "structure/topk.hpp"

using state = poe::topkdistinct<int, int, 3>;

int main() {
    std::vector<state> values{
        poe::make_topkdistinct<int, int, 3>(4, 1),
        poe::make_topkdistinct<int, int, 3>(7, 2),
        poe::make_topkdistinct<int, int, 3>(9, 1),
        poe::make_topkdistinct<int, int, 3>(5, 3)
    };
    poe::segtree<state,
                 poe::merge_topkdistinct<int, int, 3>,
                 poe::empty_topkdistinct<int, int, 3>> tree(values);
    const auto result = tree.prod(0, 4);
    assert(result.size() == 3);
    assert(result[0] == std::pair(9, 1));
    assert(result[1] == std::pair(7, 2));
    assert(result[2] == std::pair(5, 3));
}
