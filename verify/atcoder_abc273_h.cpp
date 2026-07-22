#define PROBLEM "https://atcoder.jp/contests/abc273/tasks/abc273_h"
#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/sternbrocot.hpp"
#include "tree/subarrayunion.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<pair<long long, long long>> current;
    modint998244353 answer = 0;
    const auto solve = [&] {
        if (current.empty()) return;
        const auto tree = stern_brocot_virtual_tree(current);
        vector<modint998244353> weight(tree.parent.size());
        for (int vertex = 0; vertex < static_cast<int>(tree.parent.size()); ++vertex) {
            if (tree.parent[vertex] != -1) {
                weight[vertex] = tree.depth[vertex] - tree.depth[tree.parent[vertex]];
            }
        }
        vector<int> vertices(current.size());
        iota(vertices.begin(), vertices.end(), 0);
        answer += sum_subarray_root_path_unions(tree.parent, vertices, weight);
        current.clear();
    };
    while (n--) {
        long long numerator, denominator;
        cin >> numerator >> denominator;
        if (gcd(numerator, denominator) != 1) {
            solve();
        } else {
            current.emplace_back(numerator, denominator);
        }
    }
    solve();
    cout << answer.val() << '\n';
}
