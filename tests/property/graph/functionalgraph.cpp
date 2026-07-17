#include "graph/functionalgraph.hpp"

int main() {
    std::mt19937 random(371293);
    for (int n = 1; n <= 40; ++n) {
        for (int trial = 0; trial < 100; ++trial) {
            std::vector<int> next(n);
            for (int& vertex : next) vertex = random() % n;
            poe::functionalgraph graph(next);
            for (int start = 0; start < n; ++start) {
                int vertex = start;
                for (int step = 0; step <= 3 * n; ++step) {
                    assert(graph.jump(start, step) == vertex);
                    const auto distance = graph.steps(start, vertex);
                    assert(distance && *distance <= static_cast<unsigned long long>(step));
                    vertex = next[vertex];
                }
                for (int target = 0; target < n; ++target) {
                    int current = start;
                    std::optional<unsigned long long> expected;
                    for (int step = 0; step <= 2 * n; ++step) {
                        if (current == target) {
                            expected = step;
                            break;
                        }
                        current = next[current];
                    }
                    assert(graph.steps(start, target) == expected);
                }
                assert(graph.jump(start, graph.distance_to_cycle(start)) ==
                       graph.entry(start));
            }
        }
    }
}
