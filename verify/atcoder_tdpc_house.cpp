#define PROBLEM "https://atcoder.jp/contests/tdpc/tasks/tdpc_house"
#include <bits/stdc++.h>
#include "math/matrix.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    long long floors;
    int rooms;
    cin >> floors >> rooms;
    vector<vector<int>> graph(rooms);
    for (int i = 0; i < rooms; ++i) {
        for (int j = 0; j < rooms; ++j) {
            int edge;
            cin >> edge;
            if (edge) graph[i].push_back(j);
        }
    }
    using mint = modint1000000007;
    matrix<mint> transition(rooms, rooms);
    for (int start = 0; start < rooms; ++start) {
        vector dp(1 << rooms, vector<mint>(rooms));
        dp[1 << start][start] = 1;
        for (int mask = 0; mask < (1 << rooms); ++mask) {
            for (int last = 0; last < rooms; ++last) {
                for (const int next : graph[last]) {
                    if (!(mask >> next & 1)) dp[mask | (1 << next)][next] += dp[mask][last];
                }
                transition[start][last] += dp[mask][last];
            }
        }
    }
    cout << transition.pow(floors)[0][0].val() << '\n';
}
