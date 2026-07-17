#include <bits/stdc++.h>
#include "dp/subset.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<unsigned int> adjacent(n);
    while (m--) {
        int first, second;
        cin >> first >> second;
        --first;
        --second;
        adjacent[first] |= 1U << second;
        adjacent[second] |= 1U << first;
    }

    const int count = 1 << n;
    constexpr int inf = 1'000'000;
    vector<int> cost(count, inf);
    vector<bool> clique(count);
    clique[0] = true;
    for (int mask = 1; mask < count; ++mask) {
        const int vertex = countr_zero(static_cast<unsigned int>(mask));
        const int remaining = mask ^ (1 << vertex);
        clique[mask] = clique[remaining] &&
            (adjacent[vertex] & static_cast<unsigned int>(remaining)) ==
                static_cast<unsigned int>(remaining);
        if (clique[mask]) cost[mask] = 1;
    }
    cout << subset_partition_min(n, cost, inf).cost << '\n';
}
