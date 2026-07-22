#include "dp/intervalmaximum.hpp"

using namespace poe;

int main() {
    std::vector<intervalmaximumconstraint> constraints{{0, 2, 2}, {1, 3, 3}};
    assert(count_interval_maximum_sequences<998244353>(3, 3, constraints) == 5);
    assert(count_interval_maximum_sequences<998244353>(2, 2, {}) == 9);
    assert(count_interval_maximum_sequences<998244353>(1, 1, {{0, 1, 1}}) == 1);
    assert(count_interval_maximum_sequences<998244353>(1, 1, {{0, 1, 0}}) == 1);
}
