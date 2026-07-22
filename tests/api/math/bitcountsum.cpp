#include "math/bitcountsum.hpp"

int main() {
    assert(poe::sum_popcount_arithmetic_progression(3, 1, 5) == 6);
    assert(poe::sum_popcount_arithmetic_progression(6, 1, 1) == 9);
    assert(poe::sum_popcount_arithmetic_progression(0, 0, 10) == 0);
}
