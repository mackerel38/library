#include <bits/stdc++.h>
#include "string/stacksequence.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    string source;
    cin >> n >> q >> source;
    vector<int> operations(n);
    for (int i = 0; i < n; ++i) operations[i] = source[i] == 'B' ? -1 : source[i] - '0';
    stacksequence<998244353, 10> sequence(operations);
    while (q--) {
        int type, left;
        cin >> type >> left;
        --left;
        if (type == 1) {
            char value;
            cin >> value;
            sequence.set(left, value == 'B' ? -1 : value - '0');
        } else {
            int right;
            cin >> right;
            const auto result = sequence.prod(left, right);
            if (result.empty()) cout << -1 << '\n';
            else cout << result.value << '\n';
        }
    }
}
