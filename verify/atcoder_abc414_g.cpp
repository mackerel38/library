#define PROBLEM "https://atcoder.jp/contests/abc414/tasks/abc414_g"
#include <bits/stdc++.h>
#include "graph/rangeedge.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    vector<long long> position(n);
    for (auto& value : position) cin >> value;
    vector<rangedistancetransition<long long>> transitions(m);
    for (auto& [left, right, to_left, to_right, cost] : transitions) {
        cin >> left >> right >> to_left >> to_right >> cost;
        --left;
        --to_left;
    }
    const auto distance =
        range_distance_shortest_paths(position, transitions, 0);
    for (int vertex = 1; vertex < n; ++vertex) {
        if (vertex > 1) cout << ' ';
        cout << (distance[vertex] > 4'000'000'000'000'000'000LL
            ? -1 : distance[vertex]);
    }
    cout << '\n';
}
