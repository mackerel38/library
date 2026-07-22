#include "math/qanalog.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    const poe::qcombination<mint> combination(8, mint{2});
    assert(combination.integer(3) == 7);
    assert(combination.factorial(3) == 21);
    assert(combination.choose(4, 2) == 35);
    assert(combination.choose(2, 3) == 0);

    const auto counts =
        poe::matrix_rowspace_avoiding_vector_counts<998244353>(3, 2, 2);
    assert(counts == std::vector<mint>({1, 3, 7, 15}));
}
