#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "math/modint.hpp"
#include "tree/statictoptree.hpp"

using namespace std;
using namespace poe;

using mint = modint998244353;

struct path_value {
    mint multiplier;
    mint constant;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, query_count;
    cin >> n >> query_count;
    undirected_graph<> graph(n);
    for (int vertex = 1; vertex < n; ++vertex) {
        int parent;
        cin >> parent;
        graph.add_edge(parent - 1, vertex);
    }
    vector<mint> value(n);
    for (auto& x : value) {
        int input;
        cin >> input;
        x = input;
    }

    statictoptree tree(graph);
    auto dp = make_statictoptreedp<path_value, mint>(
        tree,
        [&](int vertex) { return path_value{1, value[vertex]}; },
        [](path_value parent, path_value child) {
            return path_value{
                parent.multiplier * child.multiplier,
                parent.multiplier * child.constant + parent.constant
            };
        },
        [](mint left, mint right) { return left * right; },
        [](path_value child) { return child.constant; },
        [&](mint children, int vertex) { return path_value{children, value[vertex]}; }
    );
    while (query_count--) {
        int vertex, x;
        cin >> vertex >> x;
        value[--vertex] = x;
        dp.update(vertex);
        cout << dp.get().constant.val() << '\n';
    }
}
