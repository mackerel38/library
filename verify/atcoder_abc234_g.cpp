#define PROBLEM "https://atcoder.jp/contests/abc234/tasks/abc234_g"

#include <bits/stdc++.h>

#include "dp/partition.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int& value : a) cin >> value;
    using mint = staticmodint<998244353>;
    cout << sum_partition_range_products<mint>(a).val() << '\n';
}
