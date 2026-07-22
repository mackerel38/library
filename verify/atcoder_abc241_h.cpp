#include <bits/stdc++.h>
#include "fps/boundedgeometric.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long m;
    cin >> n >> m;
    vector<modint998244353> values(n);
    vector<long long> limits(n);
    for (int i = 0; i < n; ++i) cin >> values[i] >> limits[i];
    cout << bounded_geometric_product_coefficient(values, limits, m).val() << '\n';
}
