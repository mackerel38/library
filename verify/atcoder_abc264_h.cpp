#include <bits/stdc++.h>
#include "tree/perfectsubtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> parent(n, -1);
    for (int vertex = 1; vertex < n; ++vertex) {
        cin >> parent[vertex];
        --parent[vertex];
    }
    for (const auto count : perfect_ary_subtree_prefix_counts<998244353>(parent)) {
        cout << count << '\n';
    }
}
