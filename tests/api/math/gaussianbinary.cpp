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
}
