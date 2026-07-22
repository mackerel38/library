#include "fps/partition.hpp"
#include "math/modint.hpp"

int main() {
    const auto terms = poe::generalized_pentagonal_terms(12);
    const std::vector<poe::pentagonalterm> expected{
        {0, 1}, {1, -1}, {2, -1}, {5, 1}, {7, 1}, {12, -1}};
    assert(terms.size() == expected.size());
    for (std::size_t index = 0; index < terms.size(); ++index) {
        assert(terms[index].degree == expected[index].degree);
        assert(terms[index].coefficient == expected[index].coefficient);
    }

    using mint = poe::staticmodint<998244353>;
    const auto partitions = poe::partition_numbers<mint>(10);
    const std::vector<int> values{1, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42};
    for (int sum = 0; sum <= 10; ++sum) {
        assert(partitions[sum] == values[sum]);
    }
    assert(poe::euler_product_over_one_minus_x_coefficient<7>(5, 0) == 1);
}
