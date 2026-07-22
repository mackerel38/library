#include "graph/integerconstraints.hpp"

int main() {
    poe::boundedsumsat sat(4, 5);
    sat.add_sum_constraint(0, 2, 5, 7);
    sat.add_sum_constraint(0, 3, 1, 2);
    sat.add_sum_constraint(1, 1, 3, 8);
    assert(sat.satisfiable());
    const auto answer = sat.answer();
    assert(5 <= answer[0] + answer[2] && answer[0] + answer[2] <= 7);
    assert(1 <= answer[0] + answer[3] && answer[0] + answer[3] <= 2);
    assert(3 <= answer[1] + answer[1] && answer[1] + answer[1] <= 8);
    assert(sat[0] == answer[0]);

    poe::boundedsumsat impossible({{-2, 3}, {4, 7}});
    impossible.add_sum_upper(0, 1, 0);
    assert(!impossible.satisfiable());

    poe::boundedsumsat one_sided(2, 3);
    one_sided.add_sum_lower(0, 1, std::numeric_limits<long long>::min());
    one_sided.add_sum_upper(0, 1, std::numeric_limits<long long>::max());
    assert(one_sided.satisfiable());
}
