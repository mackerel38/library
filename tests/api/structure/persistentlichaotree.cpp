#include <cassert>
#include <cmath>

#include "structure/persistentlichaotree.hpp"

int main() {
    poe::persistentlichaotree<double, poe::lichaomode::maximum> tree(0.0, 1.0);
    const int empty = tree.initial();
    const int first = tree.add_line(empty, {1.0, 0.0});
    const int second = tree.add_line(first, {-1.0, 1.0});
    assert(std::abs(tree.get(first, 0.25) - 0.25) < 1e-12);
    assert(std::abs(tree.get(second, 0.25) - 0.75) < 1e-12);
    assert(std::abs(tree.get(first, 0.25) - 0.25) < 1e-12);
}
