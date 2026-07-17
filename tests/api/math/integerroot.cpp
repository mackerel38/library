#include <cassert>
#include <limits>

#include "math/math.hpp"

int main() {
    assert(poe::floor_sqrt(0) == 0);
    assert(poe::floor_sqrt(15) == 3);
    assert(poe::floor_sqrt(16) == 4);
    assert(poe::is_square(16));
    assert(!poe::is_square(17));
    assert(poe::floor_sqrt(std::numeric_limits<unsigned long long>::max())
           == 4294967295ULL);
}
