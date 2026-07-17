#include <cassert>
#include <vector>

#include "graph/cycle.hpp"

int main() {
    std::vector multiplicity(3, std::vector<int>(3));
    multiplicity[0][1] = multiplicity[1][0] = 1;
    multiplicity[1][2] = multiplicity[2][1] = 1;
    multiplicity[0][2] = multiplicity[2][0] = 2;
    assert(poe::count_simple_cycles<long long>(multiplicity) == 3);
}
