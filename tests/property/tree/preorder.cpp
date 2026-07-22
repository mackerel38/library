#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>

#include "tree/preorder.hpp"

long long brute(const std::vector<int>& labels) {
    const int n = labels.size();
    std::vector<int> parent(n, -1);
    long long answer = 0;
    auto enumerate = [&](auto&& self, int vertex) -> void {
        if (vertex == n) {
            std::vector<std::vector<int>> children(n);
            for (int child = 1; child < n; ++child) children[parent[child]].push_back(child);
            for (auto& list : children) {
                std::ranges::sort(list, {}, [&](int index) { return labels[index]; });
            }
            std::vector<int> order;
            auto dfs = [&](auto&& visit, int current) -> void {
                order.push_back(current);
                for (const int child : children[current]) visit(visit, child);
            };
            dfs(dfs, 0);
            std::vector<int> expected(n);
            std::iota(expected.begin(), expected.end(), 0);
            answer += order == expected;
            return;
        }
        for (parent[vertex] = 0; parent[vertex] < vertex; ++parent[vertex]) self(self, vertex + 1);
    };
    enumerate(enumerate, 1);
    return answer;
}

int main() {
    std::mt19937 random(20260722);
    for (int n = 1; n <= 8; ++n) {
        std::vector<int> labels(n);
        std::iota(labels.begin(), labels.end(), 1);
        for (int trial = 0; trial < 100; ++trial) {
            if (n > 1) std::shuffle(labels.begin() + 1, labels.end(), random);
            assert(poe::count_sorted_child_preorder_trees<long long>(labels) == brute(labels));
        }
    }
}
