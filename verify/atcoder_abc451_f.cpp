#include <bits/stdc++.h>

#include "structure/dsu.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    paritydsu uf(n);
    while (q--) {
        int u, v;
        cin >> u >> v;
        uf.merge(--u, --v, 1);
        cout << uf.minimum_color_size() << '\n';
    }
}
