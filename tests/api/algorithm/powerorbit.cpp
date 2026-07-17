#include <cassert>
#include "algorithm/powerorbit.hpp"

using namespace poe;

int main() {
    finitepowerorbit orbit(1, 2, [](int left, int right) { return left * right % 8; });
    assert(orbit.preperiod() == 3);
    assert(orbit.period() == 1);
    assert(orbit[0] == 1);
    assert(orbit[2] == 4);
    assert(orbit[1'000'000'000'000ULL] == 0);
}
