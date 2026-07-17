#include <cassert>
#include <vector>

#include "graph/generalmatching.hpp"

int main() {
    using poe::binarymatchingedge;
    assert(poe::minimum_binary_perfect_matching_weight(
        4, std::vector<binarymatchingedge>{{0, 1, 0}, {2, 3, 1}, {0, 2, 1}, {1, 3, 1}})
        == 1);
    assert(!poe::minimum_binary_perfect_matching_weight(
        4, std::vector<binarymatchingedge>{{0, 1, 0}}));
    assert(poe::minimum_binary_perfect_matching_weight(0, {}) == 0);
}
