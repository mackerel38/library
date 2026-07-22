#include <bits/stdc++.h>
#include "fps/boundedgeometric.hpp"

using namespace std;
using namespace poe;

int main() {
    vector<modint998244353> values = {3, 5, 6};
    vector<long long> limits = {1, 2, 3};
    assert(bounded_geometric_product_coefficient(values, limits, 3).val() == 819);
    assert(bounded_geometric_product_coefficient(values, limits, 7).val() == 0);
    assert(bounded_geometric_product_coefficient<998244353>({}, {}, 0).val() == 1);
}
