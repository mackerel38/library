#include "dp/decreasingconvex.hpp"
#include "tree/cartesiantree.hpp"

using namespace std;
using namespace poe;

long long fast(const vector<int>& stamina, const vector<int>& barrier) {
    const int n = static_cast<int>(stamina.size());
    const auto tree = cartesian_tree(barrier, greater{});
    vector<int> order{tree.root};
    for (int index = 0; index < static_cast<int>(order.size()); ++index) {
        const int vertex = order[index];
        if (tree.left[vertex] != -1) order.push_back(tree.left[vertex]);
        if (tree.right[vertex] != -1) order.push_back(tree.right[vertex]);
    }
    vector<decreasingconvexsequence<long long>> dp(n);
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int vertex = *iterator;
        if (tree.left[vertex] != -1) dp[vertex].merge(std::move(dp[tree.left[vertex]]));
        if (tree.right[vertex] != -1) dp[vertex].merge(std::move(dp[tree.right[vertex]]));
        dp[vertex].suffix_min(barrier[vertex], stamina[vertex]);
    }
    return dp[tree.root].zero();
}

long long brute(const vector<int>& stamina, const vector<int>& barrier) {
    const int n = static_cast<int>(stamina.size());
    vector<int> radix(n);
    int states = 1;
    for (int index = 0; index < n; ++index) {
        radix[index] = states;
        states *= stamina[index] + 1;
    }
    constexpr long long infinity = numeric_limits<long long>::max() / 4;
    vector<long long> distance(states, infinity);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> queue;
    int start = 0;
    for (int index = 0; index < n; ++index) start += stamina[index] * radix[index];
    distance[start] = 0;
    queue.emplace(0, start);
    while (!queue.empty()) {
        const auto [current_distance, state] = queue.top();
        queue.pop();
        if (distance[state] != current_distance) continue;
        if (state == 0) return current_distance;
        for (int left = 0; left < n; ++left) {
            int cost = 0;
            int next = state;
            for (int right = left; right < n; ++right) {
                cost = max(cost, barrier[right]);
                if (state / radix[right] % (stamina[right] + 1) > 0) {
                    next -= radix[right];
                }
                if (distance[next] > current_distance + cost) {
                    distance[next] = current_distance + cost;
                    queue.emplace(distance[next], next);
                }
            }
        }
    }
    unreachable();
}

int main() {
    mt19937 random(20260718);
    for (int iteration = 0; iteration < 2000; ++iteration) {
        const int n = 1 + static_cast<int>(random() % 5);
        vector<int> stamina(n), barrier(n);
        for (int& value : stamina) value = 1 + static_cast<int>(random() % 3);
        for (int& value : barrier) value = 1 + static_cast<int>(random() % 6);
        assert(fast(stamina, barrier) == brute(stamina, barrier));
    }
}
