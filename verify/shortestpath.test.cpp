#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"

#include "graph/graph.hpp"
#include "graph/shortestpath.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, start, goal;
    cin >> n >> m >> start >> goal;
    directed_graph<long long> graph(n);
    graph.read(m, 0);
    const auto result = dijkstra(graph, start);
    if (!result.reachable(goal)) {
        cout << -1 << '\n';
        return 0;
    }
    const auto path = result.path(goal);
    cout << result[goal] << ' ' << path.size() - 1 << '\n';
    for (int index = 0; index + 1 < static_cast<int>(path.size()); ++index) {
        cout << path[index] << ' ' << path[index + 1] << '\n';
    }
}
