#include <bits/stdc++.h>
#include "dp/knapsack.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, capacity;
    cin >> n >> capacity;
    vector<int> weight(n);
    vector<long long> value(n);
    for (int i = 0; i < n; ++i) cin >> weight[i] >> value[i];
    for (const auto status : classify_knapsack01_items(weight, value, capacity)) {
        cout << (status == knapsackitemstatus::mandatory ? 'A'
            : status == knapsackitemstatus::optional ? 'B' : 'C');
    }
    cout << '\n';
}
