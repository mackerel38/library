#define PROBLEM "https://atcoder.jp/contests/abc302/tasks/abc302_h"
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "structure/dsu.hpp"
#include "tree/rootpath.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<int> first(n), second(n);
    for (int vertex = 0; vertex < n; ++vertex) {
        cin >> first[vertex] >> second[vertex];
        --first[vertex];
        --second[vertex];
    }
    undirected_graph tree(n);
    for (int edge = 1; edge < n; ++edge) {
        int left, right;
        cin >> left >> right;
        tree.add_edge(left - 1, right - 1);
    }
    rollbackgraphdsu state(n, [](int vertices, int edges) {
        return min(vertices, edges);
    });
    const auto answer = root_path_rollback(
        tree, 0, state,
        [&](auto& current, int vertex) {
            current.add_edge(first[vertex], second[vertex]);
        },
        [](auto& current, int) {
            return current.value();
        }
    );
    for (int vertex = 1; vertex < n; ++vertex) {
        cout << answer[vertex] << " \n"[vertex + 1 == n];
    }
}
