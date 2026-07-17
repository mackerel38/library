#include "tree/dsuontree.hpp"

int main() {
    const std::vector<int> parent = {-1, 0, 0, 1, 1};
    const std::vector<int> value = {1, 2, 4, 8, 16};
    poe::dsuontree tree(parent);
    std::vector<int> answer(5);
    int sum = 0;
    tree.run(
        [&](int vertex) { sum += value[vertex]; },
        [&](int vertex) { sum -= value[vertex]; },
        [&](int vertex) { answer[vertex] = sum; });
    assert((answer == std::vector<int>{31, 26, 4, 8, 16}));
    assert(sum == 0);
    assert(tree.subtree_size(1) == 3 && tree.parent(1) == 0);
}
