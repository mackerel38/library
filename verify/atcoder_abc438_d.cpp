#include <bits/stdc++.h>
#include "dp/partition.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector score(3, vector<long long>(n));
    for (auto& row : score) for (auto& value : row) cin >> value;
    cout << max_phase_partition(score) << '\n';
}
