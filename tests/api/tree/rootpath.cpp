#include "graph/graph.hpp"
#include "tree/rootpath.hpp"

struct rollbacksum {
    int value = 0;
    std::vector<int> history;

    int snapshot() const {
        return history.size();
    }

    void add(int amount) {
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
    poe::undirected_graph graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(2, 3);
    const std::vector<int> value{2, 3, 5, 7};
    rollbacksum state;
    const auto answer = poe::root_path_rollback(
        graph, 0, state,
        [&](auto& current, int vertex) { current.add(value[vertex]); },
        [](auto& current, int) { return current.value; }
    );
    assert((answer == std::vector<int>{2, 5, 7, 14}));
    assert(state.value == 0 && state.history.empty());
}
