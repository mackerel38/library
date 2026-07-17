#include <bits/stdc++.h>
#include "structure/mo.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> values(n);
    for (int& value : values) cin >> value;

    int q;
    cin >> q;
    Mo mo(n);
    for (int query = 0; query < q; ++query) {
        int left, right;
        cin >> left >> right;
        mo.add(left - 1, right);
    }

    vector<int> count(n + 1), answer(q);
    int pairs = 0;
    auto add = [&](int index) {
        if (count[values[index]]++ & 1) ++pairs;
    };
    auto remove = [&](int index) {
        if (!(count[values[index]]-- & 1)) --pairs;
    };
    mo.run(add, remove, [&](int id) { answer[id] = pairs; });
    for (int value : answer) cout << value << '\n';
}
