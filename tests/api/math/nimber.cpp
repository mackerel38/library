#include "math/nimber.hpp"

int main() {
    poe::nimber a = 7;
    poe::nimber b = 11;
    assert((a + b).value == (7U ^ 11U));
    assert((a * b).value == poe::nim_product(7, 11));
    assert(a * (b + poe::nimber{3}) == a * b + a * poe::nimber{3});
}
