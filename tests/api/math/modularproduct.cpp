#include <bits/stdc++.h>
#include "math/modularproduct.hpp"

using namespace std;
using namespace poe;

int main() {
    assert(count_modular_product_sequences<998244353>(2, 3, 6) == 5);
    assert(count_modular_product_sequences<998244353>(10, 0, 2) == 1023);
    assert(count_modular_product_sequences<998244353>(0, 1, 7) == 1);
    assert(count_modular_product_sequences<998244353>(0, 0, 1) == 1);
}
