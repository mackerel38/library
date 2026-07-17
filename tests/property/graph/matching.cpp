#include "graph/matching.hpp"

int main() {
    std::mt19937 random(93817);
    for (int trial = 0; trial < 3000; ++trial) {
        const int left_size = random() % 8;
        const int right_size = random() % 8;
        poe::bipartitematching graph(left_size, right_size);
        std::vector<std::vector<bool>> edge(left_size,
                                            std::vector<bool>(right_size));
        for (int left = 0; left < left_size; ++left) {
            for (int right = 0; right < right_size; ++right) {
                if (random() % 2) {
                    edge[left][right] = true;
                    graph.add_edge(left, right);
                }
            }
        }
        std::vector<int> dp(1 << right_size, -1000);
        dp[0] = 0;
        for (int left = 0; left < left_size; ++left) {
            std::vector<int> next = dp;
            for (int mask = 0; mask < (1 << right_size); ++mask) {
                for (int right = 0; right < right_size; ++right) {
                    if (edge[left][right] && !(mask >> right & 1)) {
                        next[mask | (1 << right)] =
                            std::max(next[mask | (1 << right)], dp[mask] + 1);
                    }
                }
            }
            dp.swap(next);
        }
        const auto result = graph.solve();
        assert(result.size() == *std::max_element(dp.begin(), dp.end()));
        assert(result.left_cover.size() + result.right_cover.size() ==
               static_cast<std::size_t>(result.size()));
        std::vector<bool> cover_left(left_size), cover_right(right_size);
        for (int vertex : result.left_cover) cover_left[vertex] = true;
        for (int vertex : result.right_cover) cover_right[vertex] = true;
        assert(result.independent_size() == left_size + right_size - result.size());
        const auto independent_left = result.left_independent();
        const auto independent_right = result.right_independent();
        assert(independent_left.size() + independent_right.size() ==
               static_cast<std::size_t>(result.independent_size()));
        for (int left = 0; left < left_size; ++left) {
            for (int right = 0; right < right_size; ++right) {
                if (edge[left][right]) assert(cover_left[left] || cover_right[right]);
            }
        }
    }
}
