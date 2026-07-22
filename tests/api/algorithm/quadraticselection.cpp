#include "algorithm/quadraticselection.hpp"

using namespace poe;

int main() {
    assert(maximum_quadratic_k_selection({{1, 2}, {3, 4}, {-2, 10}}, 0) == 0);
    assert(maximum_quadratic_k_selection({{1, 2}, {3, 4}, {-2, 10}}, 1) == 14);
    assert(maximum_quadratic_k_selection({{1, 2}, {3, 4}, {-2, 10}}, 2) == 22);
    assert(maximum_quadratic_k_selection({{1, 2}, {3, 4}, {-2, 10}}, 3) == 20);
    assert(maximum_quadratic_k_selection({{2, 3}, {2, 3}, {2, 3}}, 2) == 22);
}
