#include "math/modint.hpp"
#include "math/randomwalk.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    const auto one = symmetric_walk_meeting_probabilities<998244353>({7}, 5);
    assert(ranges::all_of(one, [](mint value) { return value == 1; }));
    const auto three = symmetric_walk_meeting_probabilities<998244353>({0, 0, 2}, 1);
    assert(three == vector<mint>({0, mint{1} / 8}));
    assert(cyclic_hitting_expectation<mint>(100, 37, {1}) == 37);
    const mint half = mint{1} / 2;
    assert(cyclic_hitting_expectation<mint>(5, 1, {half, half}) == mint{34} / 11);
}
