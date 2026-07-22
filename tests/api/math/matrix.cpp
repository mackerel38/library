#include <cassert>

#include "math/matrix.hpp"

int main() {
    const int infinity = 1'000'000'000;
    poe::matrix<int> bottleneck(3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) bottleneck[i][j] = infinity;
    }
    bottleneck[0][1] = 4;
    bottleneck[1][2] = 7;
    bottleneck[0][2] = 10;
    const auto two_edges = poe::minmax_matrix_power(bottleneck, 2ULL, infinity, 0);
    assert(two_edges[0][2] == 7);

    const poe::matrix<long long> fibonacci({{1, 1}, {1, 0}});
    assert(poe::matrix_vector_product(fibonacci, std::vector<long long>{1, 0})
           == std::vector<long long>({1, 1}));
    const poe::matrix<long long> clear_first({{0, 0}, {1, 0}});
    assert(poe::sparse_matrix_transitions(
               std::vector<long long>{1, 0}, fibonacci, clear_first, 4, {2})
           == std::vector<long long>({1, 1}));

    poe::matrix<long long> cost({{3, 1}, {5, 2}});
    const auto squared = poe::minplus_matrix_power(cost, 2);
    assert(squared[0][0] == 6);
    assert(squared[0][1] == 3);
    const auto identity = poe::minplus_matrix_power(cost, 0);
    assert(identity[0][0] == 0);
    assert(identity[0][1] > 1000000000000LL);
}
