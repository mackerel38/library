#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>

#include "graph/travelingsalesman.hpp"

int main() {
    std::mt19937 random(20260715);
    constexpr long long inf = 1LL << 60;
    for (int trial = 0; trial < 200; ++trial) {
        const int n = 1 + random() % 9;
        std::vector cost(n, std::vector<long long>(n, inf));
        for (int from = 0; from < n; ++from) {
            for (int to = 0; to < n; ++to) {
                if (from != to && random() % 5 != 0) cost[from][to] = 1 + random() % 30;
            }
        }
        long long brute = n == 1 ? 0 : inf;
        std::vector<int> order(std::max(0, n - 1));
        std::iota(order.begin(), order.end(), 1);
        do {
            long long current = 0;
            int previous = 0;
            bool possible = true;
            for (const int vertex : order) {
                if (cost[previous][vertex] == inf) possible = false;
                if (possible) current += cost[previous][vertex];
                previous = vertex;
            }
            if (cost[previous][0] == inf) possible = false;
            if (possible) brute = std::min(brute, current + cost[previous][0]);
        } while (std::next_permutation(order.begin(), order.end()));
        const auto result = poe::travelingsalesman(cost, inf);
        assert(result.reachable == (brute != inf));
        if (result.reachable) assert(result.cost == brute);
    }
}
