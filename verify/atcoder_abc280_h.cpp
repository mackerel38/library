#define PROBLEM "https://atcoder.jp/contests/abc280/tasks/abc280_h"
#include <bits/stdc++.h>
#include "string/substringorder.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> strings(n);
    for (auto& text : strings) cin >> text;

    const substringorder order(strings);
    int q;
    cin >> q;
    while (q--) {
        long long rank;
        cin >> rank;
        const auto occurrence = order[rank - 1];
        cout << occurrence.string_index + 1 << ' ' << occurrence.left + 1 << ' '
             << occurrence.right << '\n';
    }
}
