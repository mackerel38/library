#include "graph/graph.hpp"
#include "tree/rootpath.hpp"

using namespace std;
using namespace poe;

struct rollbacksum {
    long long value = 0;
    vector<long long> history;

    int snapshot() const {
        return history.size();
    }

    void add(long long amount) {
        history.push_back(amount);
        value += amount;
    }

    void rollback(int snapshot) {
        while (static_cast<int>(history.size()) > snapshot) {
            value -= history.back();
            history.pop_back();
        }
    }
};

int main() {
    mt19937 random(20260716);
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 1 + random() % 100;
        undirected_graph graph(n);
        vector<int> parent(n, -1);
        for (int vertex = 1; vertex < n; ++vertex) {
            parent[vertex] = random() % vertex;
            graph.add_edge(parent[vertex], vertex);
        }
        vector<long long> value(n), expected(n);
        for (int vertex = 0; vertex < n; ++vertex) {
            value[vertex] = static_cast<int>(random() % 201) - 100;
            expected[vertex] = value[vertex] + (parent[vertex] == -1 ? 0 : expected[parent[vertex]]);
        }
        rollbacksum state;
        const auto answer = root_path_rollback(
            graph, 0, state,
            [&](auto& current, int vertex) { current.add(value[vertex]); },
            [](auto& current, int) { return current.value; }
        );
        assert(answer == expected);
        assert(state.value == 0 && state.history.empty());
    }
}
