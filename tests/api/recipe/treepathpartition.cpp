#include <cassert>

#include "recipe/treepathpartition.hpp"

int main() {
    {
        poe::undirected_graph graph(6);
        for (int vertex = 1; vertex < 6; ++vertex) graph.add_edge(vertex - 1, vertex);
        assert(poe::minimum_path_partition_width(graph) == 1);
    }
    {
        poe::undirected_graph graph(5);
        for (int vertex = 1; vertex < 5; ++vertex) graph.add_edge(0, vertex);
        assert(poe::minimum_path_partition_width(graph) == 3);
    }
    {
        poe::undirected_graph graph(7);
        graph.add_edge(2, 3);
        graph.add_edge(0, 4);
        graph.add_edge(3, 4);
        graph.add_edge(0, 1);
        graph.add_edge(6, 3);
        graph.add_edge(0, 5);
        assert(poe::minimum_path_partition_width(graph) == 3);
    }
}
