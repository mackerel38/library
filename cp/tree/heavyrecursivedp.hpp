#pragma once
#include <bits/stdc++.h>
#include "tree/rootedtree.hpp"

namespace poe {

namespace detail {

template<std::size_t Branches, class State, std::size_t... Indices>
std::array<State, Branches> filled_state_array(
    const State& state,
    std::index_sequence<Indices...>
) {
    return {((void)Indices, state)...};
}

template<std::size_t Branches, class State, class Transition, class Record>
struct heavyrecursivedp_runner {
    std::vector<std::vector<int>> children;
    Transition transition;
    Record record;

    std::array<State, Branches> run(
        int vertex,
        const State& input,
        bool record_answers
    ) {
        std::array<State, Branches> states =
            filled_state_array<Branches>(input, std::make_index_sequence<Branches>{});
        if (!children[vertex].empty()) {
            states = run(children[vertex][0], input, record_answers);
            for (std::size_t child_index = 1;
                 child_index < children[vertex].size(); ++child_index) {
                const int child = children[vertex][child_index];
                for (std::size_t branch = 0; branch < Branches; ++branch) {
                    auto next = run(child, states[branch], false);
                    states[branch] = std::move(next[branch]);
                }
            }
        }
        transition(vertex, states);
        if (record_answers) {
            record(vertex, states);
            for (std::size_t child_index = 1;
                 child_index < children[vertex].size(); ++child_index) {
                run(children[vertex][child_index], input, true);
            }
        }
        return states;
    }
};

}

/// O(f(n))。heavyrecursivedp<B>(graph, root, initial, transition, record)で分岐型部分木DPを実行する。
/// transition(vertex, states)はB個の状態を更新し、record(vertex, states)はinitial始点の各部分木結果を受け取る。
template<
    std::size_t Branches,
    undirected_graph_type Graph,
    class State,
    class Transition,
    class Record
>
std::array<State, Branches> heavyrecursivedp(
    const Graph& graph,
    int root,
    const State& initial,
    Transition transition,
    Record record
) {
    static_assert(Branches > 0);
    if (graph.size() == 0) {
        assert(root == 0);
        return detail::filled_state_array<Branches>(
            initial, std::make_index_sequence<Branches>{});
    }
    rootedtree tree(graph, root);
    std::vector<std::vector<int>> children(graph.size());
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        for (const auto& current : graph[vertex]) {
            if (tree.parent(current.to) == vertex) {
                children[vertex].push_back(current.to);
            }
        }
        std::ranges::sort(children[vertex], [&](int left, int right) {
            return tree.subtree_size(left) > tree.subtree_size(right);
        });
    }
    detail::heavyrecursivedp_runner<Branches, State, Transition, Record> runner{
        std::move(children), std::move(transition), std::move(record)};
    return runner.run(root, initial, true);
}

/// O(f(n))。recordが不要な固定根の分岐型部分木DPを実行し、rootのB状態を返す。
template<std::size_t Branches, undirected_graph_type Graph, class State, class Transition>
std::array<State, Branches> heavyrecursivedp(
    const Graph& graph,
    int root,
    const State& initial,
    Transition transition
) {
    return heavyrecursivedp<Branches>(
        graph,
        root,
        initial,
        std::move(transition),
        [](int, const std::array<State, Branches>&) {});
}

}
