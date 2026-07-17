#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_v"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "math/modint.hpp"
#include "tree/rerooting.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices, modulus;
    cin >> vertices >> modulus;
    dynamicmodint<0>::set_mod(modulus);
    using mint = dynamicmodint<0>;
    undirected_graph<> graph(vertices);
    for (int i = 1; i < vertices; ++i) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(--left, --right);
    }
    const auto answer = rerooting(
        graph, mint{1},
        [](mint left, mint right) { return left * right; },
        [](mint merged, int) { return merged; },
        [](mint child, const auto&) { return child + mint{1}; }
    );
    for (const mint value : answer) cout << value.val() << '\n';
}
