#include "dp/gridpath.hpp"

using namespace poe;

int main() {
    const std::vector<std::vector<gridpathpoint>> groups{
        {{0, 0}, {1, 1}},
        {{0, 1}, {1, 0}},
    };
    assert(count_grouped_monotone_grid_paths<998244353>(2, 2, groups) == 6);
    assert(count_grouped_monotone_grid_paths<998244353>(2, 3, {}) == 0);
    assert(count_grouped_monotone_grid_paths<998244353>(0, 0, {}) == 0);
}
