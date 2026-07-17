#define PROBLEM "https://atcoder.jp/contests/abc424/tasks/abc424_c"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/shortestpath.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    directed_graph graph(n + 1);
    for (int skill = 1; skill <= n; ++skill) {
        int first, second;
        cin >> first >> second;
        graph.add_edge(first, skill);
        graph.add_edge(second, skill);
    }
    const auto reachable = bfs(graph, 0);
    cout << ranges::count_if(
        views::iota(1, n + 1),
        [&](int skill) { return reachable.reachable(skill); }) << '\n';
}
