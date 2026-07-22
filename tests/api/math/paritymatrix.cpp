#include "math/modint.hpp"
#include "math/paritymatrix.hpp"

int main() {
    using mint = poe::staticmodint<1000000007>;
    assert(poe::count_matrices_without_even_rows<mint>(3, {3, 2}) == mint{15});
    assert(poe::count_matrices_without_even_rows<mint>(1, {1, 2}) == mint{1});
    assert(poe::count_matrices_without_even_rows<mint>(1, {2, 4}) == mint{0});
}
