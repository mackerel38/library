#include "structure/dsu.hpp"

using namespace std;
using namespace poe;

long long brute(int n, const vector<pair<int, int>>& edges) {
    vector<vector<int>> graph(n);
    for (const auto& [left, right] : edges) {
        graph[left].push_back(right);
        if (left != right) graph[right].push_back(left);
    }
    vector<char> seen(n);
    long long answer = 0;
    for (int start = 0; start < n; ++start) {
        if (seen[start]) continue;
        int vertices = 0, twice_edges = 0, loops = 0;
        vector<int> stack{start};
        seen[start] = true;
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            ++vertices;
            twice_edges += graph[vertex].size();
            for (const int to : graph[vertex]) {
                if (to == vertex) {
                    ++loops;
                } else if (!seen[to]) {
                    seen[to] = true;
                    stack.push_back(to);
                }
            }
        }
        const int edge_count = (twice_edges - loops) / 2 + loops;
        answer += 1LL * vertices * vertices + 3LL * edge_count;
    }
    return answer;
}

int main() {
    mt19937 random(20260716);
    for (int trial = 0; trial < 500; ++trial) {
        const int n = 1 + random() % 10;
        rollbackgraphdsu uf(n, [](int vertices, int edges) {
            return 1LL * vertices * vertices + 3LL * edges;
        });
        vector<pair<int, int>> edges;
        vector<pair<int, int>> checkpoints{{uf.snapshot(), 0}};
        for (int step = 0; step < 100; ++step) {
            if (checkpoints.size() > 1 && random() % 4 == 0) {
                const int keep = random() % checkpoints.size();
                uf.rollback(checkpoints[keep].first);
                edges.resize(checkpoints[keep].second);
                checkpoints.resize(keep + 1);
            } else {
                const int left = random() % n;
                const int right = random() % n;
                uf.add_edge(left, right);
                edges.emplace_back(left, right);
                checkpoints.emplace_back(uf.snapshot(), edges.size());
            }
            assert(uf.value() == brute(n, edges));
        }
    }
}
