#include <bits/stdc++.h>
#include "algorithm/groupxor.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    unsigned long long k;
    cin >> n >> c >> k;
    vector<vector<unsigned long long>> groups(c);
    for (int i = 0; i < n; ++i) {
        int color;
        unsigned long long value;
        cin >> color >> value;
        groups[color - 1].push_back(value);
    }
    cout << kth_largest_group_xor<unsigned long long, 60>(groups, k) << '\n';
}
