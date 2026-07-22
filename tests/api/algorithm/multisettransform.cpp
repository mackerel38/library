#include "algorithm/multisettransform.hpp"

using namespace poe;

int main() {
    prefixmultisetmatcher<3> matcher;
    matcher.add_left(std::vector<int>{1, 2});
    matcher.add_left(std::vector<int>{1, 0});
    matcher.add_right(std::vector<int>{1});
    matcher.add_right(std::vector<int>{1, 2, 0});
    const auto moves = matcher.minimum_moves(
        [](int) { return true; },
        [](int symbol) { return symbol == 0; }
    );
    assert(moves && *moves == 2);

    assert(minimum_double_halve_multiset_operations<unsigned>({3, 4, 5}, {2, 4, 6}) == 2);
    assert(!minimum_double_halve_multiset_operations<unsigned>({0}, {1}));
    assert(minimum_double_halve_multiset_operations<unsigned>({}, {}) == 0);
}
