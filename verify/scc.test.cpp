#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include "graph/scc.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    sccgraph graph(n);
    while (m--) {
        int from, to;
        cin >> from >> to;
        graph.add_edge(from, to);
    }
    const auto groups = graph.scc();
    cout << groups.size() << '\n';
    for (const auto& group : groups) {
        cout << group.size();
        for (int vertex : group) {
            cout << ' ' << vertex;
        }
        cout << '\n';
    }
}
