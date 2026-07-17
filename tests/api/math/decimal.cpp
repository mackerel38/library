#include <cassert>
#include <vector>

#include "math/decimal.hpp"

int main() {
    using poe::decimalrun;
    const std::vector<decimalrun> runs{{3, 1}, {1, 1}, {6, 1}, {2, 2}, {7, 2}, {6, 2}};
    assert(poe::decimal_runs_mod(runs, 1000) == 766);
    assert(poe::decimal_runs_div_mod(runs, 7, 10007) == 3797);
    assert(poe::decimal_runs_mod({decimalrun{9, 1000000000ULL}}, 9) == 0);
    assert(poe::count_divisible_concatenations({2, 42}, 11) == 2);
    assert(poe::count_divisible_concatenations({2, 8, 16, 183}, 7) == 4);
    assert(poe::smallest_multiple_containing(271, "414") == "34146");
    assert(poe::smallest_multiple_containing(15, "23") == "1230");
}
