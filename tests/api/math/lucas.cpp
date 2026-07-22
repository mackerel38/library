#include "math/lucas.hpp"

int main() {
    const poe::lucascombination<7> combination;
    assert(combination.choose(5, 2) == 3);
    assert(combination.choose(8, 1) == 1);
    assert(combination.choose(8, 2) == 0);
    assert(combination.choose(1'000'000'000'000LL, 0) == 1);
    assert(combination.choose(4, 5) == 0);
}
