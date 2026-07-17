#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DPL_2_A"
#include <bits/stdc++.h>
#include "graph/travelingsalesman.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices, edges;
    cin >> vertices >> edges;
    const long long inf = 1LL << 60;
    vector cost(vertices, vector<long long>(vertices, inf));
    while (edges--) {
        int from, to;
        long long distance;
        cin >> from >> to >> distance;
        cost[from][to] = min(cost[from][to], distance);
    }
    const auto result = travelingsalesman(cost, inf);
    cout << (result.reachable ? result.cost : -1) << '\n';
}
