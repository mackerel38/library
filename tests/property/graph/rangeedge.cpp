#include <cassert>
#include <random>
#include <vector>

#include "graph/rangeedge.hpp"

int main() {
    std::mt19937 random(414);
    using transition = poe::rangedistancetransition<long long>;
    for (int trial = 0; trial < 500; ++trial) {
        const int size = 2 + random() % 9;
        std::vector<long long> position(size);
        for (int i = 1; i < size; ++i) position[i] = position[i - 1] + 1 + random() % 8;
        std::vector<transition> transitions;
        poe::directed_graph<long long> brute(size);
        const int count = random() % 20;
        for (int query = 0; query < count; ++query) {
            int a = random() % size, b = random() % size;
            if (a == b) b = (b + 1) % size;
            if (a > b) std::swap(a, b);
            const bool east = random() & 1;
            int first_left = random() % (a + 1);
            int first_right = first_left + 1 + random() % (a - first_left + 1);
            int second_left = b;
            int second_right = b + 1 + random() % (size - b);
            if (!east) {
                std::swap(first_left, second_left);
                std::swap(first_right, second_right);
            }
            const long long cost = random() % 20;
            transitions.push_back(
                {first_left, first_right, second_left, second_right, cost});
            for (int from = first_left; from < first_right; ++from) {
                for (int to = second_left; to < second_right; ++to) {
                    brute.add_edge(
                        from, to, cost + std::abs(position[from] - position[to]));
                }
            }
        }
        const int start = random() % size;
        const auto actual =
            poe::range_distance_shortest_paths(position, transitions, start);
        const auto expected = poe::dijkstra(brute, start);
        for (int vertex = 0; vertex < size; ++vertex) {
            assert((actual[vertex] > 1'000'000) == !expected.reachable(vertex));
            if (expected.reachable(vertex)) assert(actual[vertex] == expected[vertex]);
        }
    }
}
