#include "math/sternbrocot.hpp"

int main() {
    const auto path = poe::stern_brocot_path(3, 7);
    assert(path.depth == 4);
    assert((path.runs == std::vector<std::pair<bool, long long>>{{false, 2}, {true, 2}}));

    const auto tree = poe::stern_brocot_virtual_tree({{1, 2}, {3, 7}, {2, 1}});
    assert(tree.leaves == 3 && tree.root >= 3);
    for (int leaf = 0; leaf < tree.leaves; ++leaf) assert(tree.parent[leaf] != -1);
}
