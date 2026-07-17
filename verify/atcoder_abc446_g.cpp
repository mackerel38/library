#include <bits/stdc++.h>
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> value(n), previous_count(n);
    vector<vector<int>> positions(n + 1);
    for (int i = 0; i < n; ++i) {
        cin >> value[i];
        previous_count[i] = positions[value[i]].size();
        positions[value[i]].push_back(i);
    }

    using mint = staticmodint<998244353>;
    vector<mint> prefix(n + 1);
    for (int i = 0; i < n; ++i) {
        mint current = 0;
        const int count = previous_count[i];
        const int block = value[i];
        if (count == block - 1) {
            const int right = positions[block][count - block + 1];
            current = prefix[right] + 1;
        } else if (count >= block) {
            const int left = positions[block][count - block];
            const int right = positions[block][count - block + 1];
            current = prefix[right] - prefix[left + 1];
        }
        prefix[i + 1] = prefix[i] + current;
    }
    cout << prefix[n].val() << '\n';
}
