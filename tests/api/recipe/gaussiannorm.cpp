#include <cassert>
#include <utility>
#include <vector>
#include "recipe/gaussiannorm.hpp"
#include "math/modint.hpp"

using namespace poe;

int main() {
    const std::vector<std::pair<int, unsigned long long>> factors{{2, 1}, {5, 2}};
    const auto count = gaussian_norm_residue_distribution<modint998244353>(factors, 2);
    assert(count[3].val() == 12);
}
