#include "flow/lowerbound.hpp"
#include "flow/submodular.hpp"

struct bounded_edge {
    int from;
    int to;
    int lower;
    int upper;
};

std::vector<std::pair<int, std::vector<int>>> brute_flows(
    int n,
    const std::vector<bounded_edge>& edges,
    int source,
    int sink
) {
    std::vector<std::pair<int, std::vector<int>>> result;
    std::vector<int> flow(edges.size());
    auto search = [&](auto&& self, int index) -> void {
        if (index != static_cast<int>(edges.size())) {
            for (flow[index] = edges[index].lower; flow[index] <= edges[index].upper;
                 ++flow[index]) {
                self(self, index + 1);
            }
            return;
        }
        std::vector<int> balance(n);
        for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
            balance[edges[i].from] -= flow[i];
            balance[edges[i].to] += flow[i];
        }
        for (int vertex = 0; vertex < n; ++vertex) {
            if (vertex != source && vertex != sink && balance[vertex] != 0) {
                return;
            }
        }
        const int value = -balance[source];
        if (value >= 0 && balance[sink] == value) {
            result.emplace_back(value, flow);
        }
    };
    search(search, 0);
    return result;
}

int main() {
    std::mt19937 random(314159);
    for (int iteration = 0; iteration < 400; ++iteration) {
        const int n = 2 + random() % 3;
        const int m = 1 + random() % 5;
        std::vector<bounded_edge> edges;
        poe::lowerboundflow<int> problem(n);
        for (int i = 0; i < m; ++i) {
            int from = random() % n;
            int to = random() % n;
            while (from == to) {
                to = random() % n;
            }
            const int lower = random() % 3;
            const int upper = lower + random() % 3;
            edges.push_back({from, to, lower, upper});
            problem.add_edge(from, to, lower, upper);
        }
        const auto expected = brute_flows(n, edges, 0, n - 1);
        const auto maximum = problem.maxflow(0, n - 1);
        const auto minimum = problem.minflow(0, n - 1);
        if (expected.empty()) {
            assert(!maximum);
            assert(!minimum);
            continue;
        }
        const auto [min_it, max_it] = std::minmax_element(
            expected.begin(), expected.end(),
            [](const auto& left, const auto& right) { return left.first < right.first; }
        );
        assert(maximum && maximum->flow == max_it->first);
        assert(minimum && minimum->flow == min_it->first);
        assert(std::find(expected.begin(), expected.end(),
                         std::pair(maximum->flow, maximum->edge_flow)) != expected.end());
        assert(std::find(expected.begin(), expected.end(),
                         std::pair(minimum->flow, minimum->edge_flow)) != expected.end());
    }

    for (int iteration = 0; iteration < 500; ++iteration) {
        const int n = 1 + random() % 7;
        poe::submodular<long long> function(n);
        std::vector<std::array<long long, 3>> unary;
        std::vector<std::array<long long, 6>> pair;
        for (int i = 0; i < n; ++i) {
            const long long cost0 = static_cast<int>(random() % 9) - 4;
            const long long cost1 = static_cast<int>(random() % 9) - 4;
            unary.push_back({i, cost0, cost1});
            function.add_unary(i, cost0, cost1);
        }
        for (int k = 0; k < n * 2; ++k) {
            int i = random() % n;
            int j = random() % n;
            if (i == j) {
                continue;
            }
            const long long c = static_cast<int>(random() % 9) - 4;
            const long long a = static_cast<int>(random() % 9) - 4;
            const long long b = static_cast<int>(random() % 9) - 4;
            const long long w = random() % 6;
            const long long c00 = c;
            const long long c01 = c + b + w;
            const long long c10 = c + a;
            const long long c11 = c + a + b;
            pair.push_back({i, j, c00, c01, c10, c11});
            function.add_pair(i, j, c00, c01, c10, c11);
        }
        long long expected = std::numeric_limits<long long>::max();
        auto evaluate = [&](int mask) {
            long long value = 0;
            for (const auto& term : unary) {
                value += term[1 + (mask >> term[0] & 1)];
            }
            for (const auto& term : pair) {
                const int x = mask >> term[0] & 1;
                const int y = mask >> term[1] & 1;
                value += term[2 + x * 2 + y];
            }
            return value;
        };
        for (int mask = 0; mask < (1 << n); ++mask) {
            expected = std::min(expected, evaluate(mask));
        }
        const auto actual = function.minimize();
        int mask = 0;
        for (int i = 0; i < n; ++i) {
            mask |= static_cast<int>(actual.value[i]) << i;
        }
        assert(actual.cost == expected);
        assert(evaluate(mask) == expected);
    }
}
