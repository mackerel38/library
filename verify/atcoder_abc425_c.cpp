#define PROBLEM "https://atcoder.jp/contests/abc425/tasks/abc425_c"
#include <bits/stdc++.h>
#include "structure/prefixsum.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> values(n);
    for (auto& value : values) cin >> value;
    circularprefixsum<long long> data(values);

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            long long count;
            cin >> count;
            data.rotate(count);
        } else {
            int left, right;
            cin >> left >> right;
            cout << data.sum(left - 1, right) << '\n';
        }
    }
}
