#include <cassert>
#include <vector>

#include "structure/xortrie.hpp"

int main() {
    poe::xortrie<unsigned, 4> trie;
    trie.add(1);
    trie.add(2);
    trie.add(2);
    assert(trie.count(2) == 2);
    assert(trie.count_xor_less(3, 2) == 2);
    assert(trie.count_xor_at_most(0, 2) == 3);
    assert(trie.min_xor(3) == 1);
    trie.erase(2);
    assert(trie.size() == 2);
    assert((poe::count_pairs_xor_at_most<unsigned, 4>({1, 2, 3}, 1) == 1));
    assert((poe::sum_min_xor<unsigned, 2>({1, 2}, 4) == 2));
    assert((poe::sum_min_xor<unsigned, 2>({1, 2}, 1, 4) == 1));
}
