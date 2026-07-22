#include <cassert>
#include <vector>
#include "math/gaussian.hpp"

using namespace std;
using namespace poe;

int main() {
    matrix<int> coefficients({{1, 1, 0}, {0, 1, 1}});
    auto result = solve_binary_linear(coefficients, {1, 0});
    assert(result.status == linearstatus::multiple);
    assert(result.rank == 2 && result.basis.size() == 1);
    assert(result.solution == vector<int>({1, 0, 0}));
    assert(result.basis[0] == vector<int>({1, 1, 1}));
    assert(binary_matrix_rank(coefficients) == 2);

    matrix<int> impossible(vector<vector<int>>{{1}, {1}});
    assert(solve_binary_linear(impossible, {0, 1}).status == linearstatus::none);

    auto one = solve_sparse_binary_linear_one(3, {{0, 1}, {1, 2}}, {1, 0});
    assert(one && ((*one)[0] ^ (*one)[1]) == 1);
    assert(((*one)[1] ^ (*one)[2]) == 0);
    assert(!solve_sparse_binary_linear_one(1, {{0}, {0}}, {0, 1}));
}
