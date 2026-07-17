#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_p"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "math/modint.hpp"
#include "tree/dp.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices;
    cin >> vertices;
    undirected_graph<> graph(vertices);
    for (int edge = 1; edge < vertices; ++edge) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(--left, --right);
    }
    using mint = modint1000000007;
    using state = array<mint, 2>;
    const auto dp = treedp(
        graph, 0, state{1, 1},
        [](state left, state right) { return state{left[0] * right[0], left[1] * right[1]}; },
        [](state children, int) { return children; },
        [](state child, const auto&) { return state{child[0] + child[1], child[0]}; }
    );
    cout << (dp[0][0] + dp[0][1]).val() << '\n';
}
