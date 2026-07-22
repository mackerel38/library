#include <cassert>

#include "math/xorbasis.hpp"

int main() {
    poe::xorbasis<> basis;
    assert(basis.add(3));
    assert(basis.add(5));
    assert(!basis.add(6));
    assert(basis.rank() == 2 && basis.contains(6));
    assert(basis.max() == 6);
    assert(basis.kth(0) == 0 && basis.kth(3) == 6);
    const std::vector<std::pair<unsigned, unsigned>> paired{{1, 1}, {3, 2}, {2, 2}, {0, 1}};
    assert((poe::maximum_paired_xor_under<unsigned, 2>(paired, 2) == 3));
    assert(!(poe::maximum_paired_xor_under<unsigned, 2>({{3, 0}}, 2).has_value()));
}
