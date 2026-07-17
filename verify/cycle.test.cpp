#define PROBLEM "https://judge.yosupo.jp/problem/cycle_detection"

#include "graph/cycle.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    directed_graph<> graph(n);
    for (int index = 0; index < m; ++index) {
        int from, to;
        cin >> from >> to;
        graph.add_edge(from, to);
    }
    const auto result = find_cycle(graph);
    if (!result.found()) {
        cout << -1 << '\n';
        return 0;
    }
    cout << result.edge_ids.size() << '\n';
    for (int id : result.edge_ids) cout << id << '\n';
}
