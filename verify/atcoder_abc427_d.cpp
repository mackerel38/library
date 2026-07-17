#define PROBLEM "https://atcoder.jp/contests/abc427/tasks/abc427_d"
#include <bits/stdc++.h>
#include "game/finite.hpp"

using namespace std;
using namespace poe;

int main() {
    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n, m, k;
        cin >> n >> m >> k;
        string labels;
        cin >> labels;
        directed_graph graph(n);
        while (m--) {
            int from, to;
            cin >> from >> to;
            graph.add_edge(from - 1, to - 1);
        }
        vector<bool> terminal(n);
        for (int vertex = 0; vertex < n; ++vertex) {
            terminal[vertex] = labels[vertex] == 'A';
        }
        const auto result = finite_horizon_game(graph, terminal, 2 * k);
        cout << (result[0] ? "Alice" : "Bob") << '\n';
    }
}
