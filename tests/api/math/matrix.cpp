#include <cassert>

#include "math/matrix.hpp"

int main() {
    poe::matrix<long long> cost({{3, 1}, {5, 2}});
    const auto squared = poe::minplus_matrix_power(cost, 2);
    assert(squared[0][0] == 6);
    assert(squared[0][1] == 3);
    const auto identity = poe::minplus_matrix_power(cost, 0);
    assert(identity[0][0] == 0);
    assert(identity[0][1] > 1000000000000LL);
}
