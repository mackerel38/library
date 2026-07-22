#include "graph/unlabeledcount.hpp"
#include "math/modint.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    assert(poe::count_surjectively_colored_unlabeled_graphs<mint>(3, 1) == mint{4});
    assert(poe::count_surjectively_colored_unlabeled_graphs<mint>(3, 2) == mint{12});
    assert(poe::count_surjectively_colored_unlabeled_graphs<mint>(5, 5) == mint{1024});
}
