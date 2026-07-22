#include "string/xorrollinghash.hpp"

int main() {
    poe::xorrollinghash hash({1, 2, 3, 1}, 123456789);
    assert(hash.size() == 4 && hash[2] == 3);
    assert(hash.xor_hash(0, 2, 1, 3) ==
           (hash.hash(0, 2) ^ hash.hash(1, 3)));
    assert(hash.xor_lcp(0, 1, 2, 2, 4) == 2);
    poe::xorrollinghash other({2, 3}, hash.base());
    poe::xorrollinghash target({3, 1}, hash.base());
    assert(hash.xor_lcp(0, 2, other, 0, 2, target, 0, 2) == 2);
}
