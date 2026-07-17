#include <cassert>

#include "dp/concave.hpp"

int main() {
    poe::concavesequence sequence(4, 0);
    sequence.add_affine(-2, 16);
    sequence.trim_below(0);
    sequence.suffix_max(3, 0);
    sequence.trim_below(0);
    assert(sequence.left() == 2 && sequence.right() == 4);
    assert(!sequence.get(0));
    assert(!sequence.get(1));
    assert(sequence.get(2) == 2);
    assert(sequence.get(4) == 8);
    assert(!sequence.get(5));
}
