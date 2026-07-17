#include <cassert>

#include "structure/setsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    setsegtree tree(4);
    tree.add(0, 2, 10);
    tree.add(1, 4, 20);
    assert(tree.max(0, 3).size == 2);
    assert(tree.max(0, 3).count == 1);
    tree.erase(0, 2, 20);
    tree.add(1, 3, 10);
    assert(tree.max(0, 2).size == 1);
    assert(tree.max(0, 2).count == 2);
    assert(popcount(tree[2]) == 2);
}
