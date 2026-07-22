#include "graph/spanningforest.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    mt19937 random(2531);
    for (int iteration = 0; iteration < 3000; ++iteration) {
        const int n = 1 + random() % 6;
        matrix<mint> weights(n, n);
        vector<tuple<int, int, int>> edges;
        for (int left = 0; left < n; ++left) {
            for (int right = left + 1; right < n; ++right) {
                const int weight = random() % 4;
                weights[left][right] = weights[right][left] = weight;
                if (weight != 0) edges.emplace_back(left, right, weight);
            }
        }

        vector<mint> expected(n);
        for (int subset = 0; subset < (1 << edges.size()); ++subset) {
            vector<int> parent(n, -1);
            auto leader = [&](auto&& self, int vertex) -> int {
                return parent[vertex] < 0 ? vertex : parent[vertex] = self(self, parent[vertex]);
            };
            mint ways = 1;
            int count = 0;
            bool forest = true;
            for (int edge = 0; edge < static_cast<int>(edges.size()); ++edge) {
                if (!(subset >> edge & 1)) continue;
                const auto [left, right, weight] = edges[edge];
                int a = leader(leader, left), b = leader(leader, right);
                if (a == b) {
                    forest = false;
                    break;
                }
                if (parent[a] > parent[b]) swap(a, b);
                parent[a] += parent[b];
                parent[b] = a;
                ways *= weight;
                ++count;
            }
            if (forest) expected[count] += ways;
        }
        assert(spanning_forest_counts(weights) == expected);
    }
}
