#include <cassert>
#include <random>

#include "flow/mincost.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(421);
    using edge = costtransportedge<int, long long>;
    for (int test = 0; test < 2000; ++test) {
        const int size = random() % 4 + 1;
        const int edge_count = random() % 5;
        vector<int> supply(size), demand(size);
        vector<edge> edges(edge_count);
        for (int& value : supply) value = random() % 3;
        for (int& value : demand) value = random() % 3;
        for (auto& current : edges) {
            current = {static_cast<int>(random() % size), static_cast<int>(random() % size),
                       static_cast<int>(random() % 3), static_cast<long long>(random() % 5)};
        }

        optional<long long> expected;
        vector<int> flow(edge_count), incoming(size), outgoing(size);
        auto enumerate = [&](auto&& self, int index) -> void {
            if (index < edge_count) {
                for (flow[index] = 0; flow[index] <= edges[index].capacity; ++flow[index]) {
                    self(self, index + 1);
                }
                return;
            }
            fill(incoming.begin(), incoming.end(), 0);
            fill(outgoing.begin(), outgoing.end(), 0);
            long long cost = 0;
            for (int id = 0; id < edge_count; ++id) {
                outgoing[edges[id].from] += flow[id];
                incoming[edges[id].to] += flow[id];
                cost += flow[id] * edges[id].cost;
            }
            for (int vertex = 0; vertex < size; ++vertex) {
                const int used_supply = demand[vertex] + outgoing[vertex] - incoming[vertex];
                if (used_supply < 0 || used_supply > supply[vertex]) return;
            }
            if (!expected || cost < *expected) expected = cost;
        };
        enumerate(enumerate, 0);
        const auto actual = mincost_transport<int, long long>(supply, demand, edges);
        assert(actual.feasible() == expected.has_value());
        if (expected) assert(actual.cost == *expected);
    }
}
