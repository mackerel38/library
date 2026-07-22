#include "math/lucas.hpp"

template <int prime>
void test_prime() {
    const poe::lucascombination<prime> combination;
    std::vector<int> row(1, 1);
    for (int n = 0; n <= 500; ++n) {
        for (int k = 0; k <= n; ++k) {
            assert(combination.choose(n, k).val() == static_cast<unsigned int>(row[k]));
        }
        std::vector<int> next(static_cast<std::size_t>(n) + 2, 1);
        for (int k = 1; k <= n; ++k) {
            next[k] = (row[k - 1] + row[k]) % prime;
        }
        row.swap(next);
    }
}

int main() {
    test_prime<2>();
    test_prime<3>();
    test_prime<5>();
    test_prime<7>();
    test_prime<11>();
    test_prime<97>();
}
