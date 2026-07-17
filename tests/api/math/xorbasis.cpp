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
}
