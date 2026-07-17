#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"

#include "graph/graph.hpp"
#include "structure/fenwicktree.hpp"
#include "tree/heavylightdecomposition.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, query_count;
    cin >> n >> query_count;
    vector<long long> values(n);
    for (long long& value : values) cin >> value;
    undirected_graph<> graph(n);
    for (int index = 1; index < n; ++index) {
        int left, right;
        cin >> left >> right;
        graph.add_edge(left, right);
    }
    hld tree(graph, 0);
    vector<long long> ordered(n);
    for (int vertex = 0; vertex < n; ++vertex) ordered[tree[vertex]] = values[vertex];
    fenwicktree<long long> sum(ordered);
    while (query_count--) {
        int type;
        cin >> type;
        if (type == 0) {
            int vertex;
            long long value;
            cin >> vertex >> value;
            sum.add(tree[vertex], value);
        } else {
            int from, to;
            cin >> from >> to;
            long long answer = 0;
            for (const auto& segment : tree.path(from, to)) {
                answer += sum.sum(segment.left, segment.right);
            }
            cout << answer << '\n';
        }
    }
}
