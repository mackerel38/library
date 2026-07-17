#include <bits/stdc++.h>
#include "graph/cycle.hpp"
#include "graph/graph.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n, m;
        cin >> n >> m;
        vector<pair<int, int>> roads(m);
        for (auto& [u, v] : roads) {
            cin >> u >> v;
            --u;
            --v;
        }
        int week;
        cin >> week;
        vector<string> holiday(n);
        for (string& days : holiday) cin >> days;

        directed_graph<> graph(n * week);
        graph.reserve_edges((n + 2 * m) * week);
        auto id = [week](int city, int day) { return city * week + day; };
        for (int day = 0; day < week; ++day) {
            const int next = (day + 1) % week;
            for (int city = 0; city < n; ++city) {
                if (holiday[city][day] == 'o' && holiday[city][next] == 'o') {
                    graph.add_edge(id(city, day), id(city, next));
                }
            }
            for (auto [u, v] : roads) {
                if (holiday[u][day] == 'o' && holiday[v][next] == 'o') {
                    graph.add_edge(id(u, day), id(v, next));
                }
                if (holiday[v][day] == 'o' && holiday[u][next] == 'o') {
                    graph.add_edge(id(v, day), id(u, next));
                }
            }
        }
        cout << (find_cycle(graph).found() ? "Yes" : "No") << '\n';
    }
}
