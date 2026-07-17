#include <cassert>
#include <vector>

#include "tree/treemetric.hpp"

int main() {
    std::vector<std::vector<int>> distance{{0, 2, 5, 4}, {2, 0, 3, 2},
                                           {5, 3, 0, 5}, {4, 2, 5, 0}};
    auto tree = poe::restore_tree_metric(distance);
    assert(tree && tree->edge_count() == 3);
    distance[2][3] = distance[3][2] = 6;
    assert(!poe::is_tree_metric(distance));
}
