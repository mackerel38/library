#define PROBLEM "https://atcoder.jp/contests/abc274/tasks/abc274_h"
#include <bits/stdc++.h>
#include "string/xorrollinghash.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<uint64_t> values(n);
    for (auto& value : values) cin >> value;
    xorrollinghash hash(values);
    while (q--) {
        int a, b, c, d, e, f;
        cin >> a >> b >> c >> d >> e >> f;
        --a;
        --c;
        --e;
        const int length = b - a;
        const int common = hash.xor_lcp(a, c, length, e, f);
        const bool less = e + common != f &&
            (common == length ||
             (values[a + common] ^ values[c + common]) < values[e + common]);
        cout << (less ? "Yes\n" : "No\n");
    }
}
