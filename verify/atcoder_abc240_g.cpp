#include <bits/stdc++.h>
#include "math/latticewalk.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x, y, z;
    cin >> n >> x >> y >> z;
    axiswalkcounter<998244353> counter(n);
    cout << counter.count(n, x, y, z).val() << '\n';
}
