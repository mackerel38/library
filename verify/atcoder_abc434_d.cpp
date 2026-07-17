#include <bits/stdc++.h>
#include "structure/prefixsum.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    constexpr int size = 2000;
    int n;
    cin >> n;
    differencearray2d<int> count(size, size);
    differencearray2d<long long> owner(size, size);
    for (int id = 1; id <= n; ++id) {
        int upper, lower, left, right;
        cin >> upper >> lower >> left >> right;
        count.add(upper - 1, left - 1, lower, right, 1);
        owner.add(upper - 1, left - 1, lower, right, id);
    }
    count.build();
    owner.build();
    vector<long long> unique(n + 1);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            if (count(row, column) == 0) ++unique[0];
            else if (count(row, column) == 1) ++unique[owner(row, column)];
        }
    }
    for (int id = 1; id <= n; ++id) cout << unique[0] + unique[id] << '\n';
}
