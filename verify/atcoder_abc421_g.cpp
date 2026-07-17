#define PROBLEM "https://atcoder.jp/contests/abc421/tasks/abc421_g"
#include <bits/stdc++.h>
#include "flow/mincost.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, interval_count;
    cin >> n >> interval_count;
    vector<int> values(n);
    for (int& value : values) cin >> value;

    vector<int> supply(n + 1), demand(n + 1);
    int total_demand = 0;
    for (int index = 0; index < n; ++index) {
        const int difference = values[index] - (index == 0 ? 0 : values[index - 1]);
        if (difference >= 0) supply[index] = difference;
        else demand[index] = -difference, total_demand -= difference;
    }
    supply[n] = total_demand;

    using edge = costtransportedge<int, long long>;
    vector<edge> edges;
    while (interval_count--) {
        int left, right;
        cin >> left >> right;
        edges.push_back({right, left - 1, total_demand, 1});
    }
    const auto answer = mincost_transport<int, long long>(supply, demand, edges);
    cout << (answer.feasible() ? answer.cost : -1) << '\n';
}
