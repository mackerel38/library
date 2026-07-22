#include <bits/stdc++.h>
#include "dp/subsetsumcount.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, target;
    cin >> n >> target;
    vector<int> values(n);
    for (int& value : values) cin >> value;
    cout << subset_sum_parity_counts<998244353>(values, target).odd[target] << '\n';
}
