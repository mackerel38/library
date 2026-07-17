#include <cassert>

#include "math/modularsum.hpp"

using namespace std;
using namespace poe;

int main() {
    assert(sum_power_mod_linear(4, 5, 2, 1, 2, 1000000000) == 15);
    assert(sum_power_mod_linear(777, 429, 33, 58, 1, 1000000000) == 777);
    assert(sum_power_mod_linear(10, 7, 0, 3, 6, 8) == 0);
    assert(sum_power_mod_linear(10, 7, 0, 3, 0, 1) == 0);
}
