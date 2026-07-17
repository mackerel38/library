#include <cassert>
#include "math/modint.hpp"
#include "structure/affinemap.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    poe::affinemap<int, mint> values;
    values.set(2, 10);
    values.set(5, 30);
    values.apply(-1, 100);
    assert(values.get(2) == mint(90));
    assert(values.sum() == mint(160));
    values.erase(5);
    assert(values.size() == 1);
}
