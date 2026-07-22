#include "fps/partition.hpp"
#include "math/modint.hpp"

int main() {
    constexpr int limit = 300;
    std::vector<long long> euler(limit + 1);
    euler[0] = 1;
    for (int part = 1; part <= limit; ++part) {
        for (int sum = limit; sum >= part; --sum) {
            euler[sum] -= euler[sum - part];
        }
    }
    std::vector<long long> sparse(limit + 1);
    for (const auto [degree, coefficient] :
         poe::generalized_pentagonal_terms(limit)) {
        sparse[degree] = coefficient;
    }
    assert(euler == sparse);

    using mint = poe::staticmodint<101>;
    const auto partitions = poe::partition_numbers<mint>(limit);
    std::vector<mint> brute(limit + 1);
    brute[0] = 1;
    for (int part = 1; part <= limit; ++part) {
        for (int sum = part; sum <= limit; ++sum) {
            brute[sum] += brute[sum - part];
        }
    }
    assert(partitions == brute);

    for (int power = 0; power <= 20; ++power) {
        std::vector<mint> denominator(limit + 1);
        denominator[0] = 1;
        for (int count = 0; count < power; ++count) {
            for (int degree = 1; degree <= limit; ++degree) {
                denominator[degree] += denominator[degree - 1];
            }
        }
        for (int degree = 0; degree <= limit; ++degree) {
            mint expected = 0;
            for (int left = 0; left <= degree; ++left) {
                expected += mint{euler[left]} * denominator[degree - left];
            }
            assert((poe::euler_product_over_one_minus_x_coefficient<101>(degree,
                                                                         power) ==
                    expected));
        }
    }
}
