#include "recipe/compassmove.hpp"

using namespace poe;

int main() {
    const compassmoves cardinal("10101010");
    assert(cardinal.distance(5, 3) == 8);
    const compassmoves diagonal("01010101");
    assert(diagonal.distance(5, 3) == 5);
    assert(!compassmoves("10001111").distance(0, 1));
    assert(compassmoves(0U).distance(0, 0) == 0);
}
