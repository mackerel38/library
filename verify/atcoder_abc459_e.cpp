#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<int>> children(n);
    for (int vertex = 1; vertex < n; ++vertex) {
        int parent;
        cin >> parent;
        children[--parent].push_back(vertex);
    }
    vector<long long> candy(n), take(n);
    int total_take = 0;
    for (auto& value : candy) cin >> value;
    for (auto& value : take) {
        cin >> value;
        total_take += value;
    }

    combination<modint998244353> comb(total_take);
    modint998244353 answer = 1;
    vector<long long> remaining(n);
    vector<int> order{0};
    for (int i = 0; i < static_cast<int>(order.size()); ++i) {
        for (const int child : children[order[i]]) order.push_back(child);
    }
    reverse(order.begin(), order.end());
    for (const int vertex : order) {
        remaining[vertex] += candy[vertex];
        for (const int child : children[vertex]) remaining[vertex] += remaining[child];
        answer *= comb.choose_large(remaining[vertex], static_cast<int>(take[vertex]));
        remaining[vertex] -= take[vertex];
    }
    cout << answer << '\n';
}
