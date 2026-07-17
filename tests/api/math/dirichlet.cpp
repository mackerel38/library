#include <cassert>

#include "math/dirichlet.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    vector<mint> frequency(7);
    frequency[2] = 2;
    frequency[3] = 1;
    frequency[4] = 1;
    const auto answer = product_necklace_counts(frequency);
    const vector<int> expected = {0, 0, 2, 1, 4, 0, 2};
    for (int value = 2; value <= 6; ++value) assert(answer[value] == mint{expected[value]});

    vector<mint> values(9);
    values[1] = 2;
    values[2] = 3;
    values[4] = 5;
    const auto inverse = dirichlet_inverse(values);
    const auto identity = dirichlet_convolution(values, inverse);
    assert(identity[1] == mint{1});
    for (int value = 2; value <= 8; ++value) assert(identity[value] == mint{0});
}
